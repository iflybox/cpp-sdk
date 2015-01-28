#ifndef CINICONFIG_BY_ZWZHAO_2005_11_09
#define CINICONFIG_BY_ZWZHAO_2005_11_09

#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include "cfgunit.h"
#include "base/srbase.h"
#include "utils/ospath.hpp"
#include "iFly_Assert.h"

// 灭掉warning 4996
#if _MSC_VER > 1300
#  pragma warning(push)
#  pragma warning(disable : 4996)
#endif /* _MSC_VER > 1300 */

#if !defined (WIN32)
#define stricmp strcasecmp
#endif

const int MAX_KEY_LENGTH = 256;
const int MAX_VALUE_LENGTH = 256;

enum config_data_type
{
	type_dword = 0,
	type_string = 1,
	type_bool = 2,
};

//返回值设定
const int CONFIG_ERR_OK = 0;
const int CONFIG_ERR_COMMON = -1;
const int CONFIG_ERR_FILEOPEN = -1;

enum config_type
{
	config_reg = 0,
	config_ini = 1,
	config_xml = 2,
};

enum text_code_type
{
	config_utf8 = 0,
	config_unicode = 1,
};

#ifdef _MSC_VER
const char _cfg_dirname[] = "lib";
#else
const char _cfg_dirname[] = "etc";
#endif	// _MSC_VER

typedef struct tag_config_enum_data
{
public:
	tag_config_enum_data()
	{
		memset(value_name,0,MAX_VALUE_LENGTH+1);
		data_type = type_dword;
	}

	~tag_config_enum_data()
	{
	}
	char value_name[MAX_VALUE_LENGTH+1];
	config_data_type data_type;
	std::string data_value;

	int orig_index;
	int file_index;

	bool operator < (const tag_config_enum_data &p1) const
	{
		return stricmp(value_name,p1.value_name) < 0;
	}

	static bool less_by_index(const tag_config_enum_data &p1, const tag_config_enum_data &p2)
	{
		return p1.orig_index < p2.orig_index;
	}
}config_enum_data;

class cfgunit_utility
{
public:
	//根据配置文件中的一行字符串创建值项
	static int create_value_str(const config_enum_data &config_data,char*value_str)
	{
		assert(value_str);
		assert(config_data.data_type == type_string);

		if(config_data.data_type == type_string)
		{
			sprintf(value_str,"%s=%s",config_data.value_name,config_data.data_value.c_str());
		}
/*
		else if(config_data.data_type == type_dword || config_data.data_type == type_bool )
		{
			assert(0);
			sprintf(value_str,"%s=%08d",config_data.value_name,config_data.value.nvalue);
		}
*/

		//替换,将"\"换成"\\"
		std::string temp_str = value_str;
//		string_replace(temp_str,"\\","\\\\");

		strcpy(value_str,temp_str.c_str());
		return CONFIG_ERR_OK;
	}

	//一个string的替换功能
	static void string_replace(std::string & entire_string, const std::string & replace_src, const std::string &replace_dst) 
	{
		std::string::size_type pos=0;
		std::string::size_type srclen=replace_src.size();
		std::string::size_type dstlen=replace_dst.size();
		while( (pos=entire_string.find(replace_src, pos)) != std::string::npos){
			entire_string.replace(pos, srclen, replace_dst);
			pos += dstlen;
		}
		return;
	}
};

class cfgunit_ini : cfgunit
{
public:
	cfgunit_ini()
	{
		keyname_ = "";
		value_set_.clear();
		child_set_.clear();
		parent_node_ = NULL;

		file_index_ = 0;
		orig_index_ = 0;

	}

	cfgunit_ini(const char *keyname,cfgunit_ini *parent_node)
	{
		keyname_ = keyname;
		value_set_.clear();
		child_set_.clear();
		parent_node_ = parent_node;

		char full_name[256] = { 0 };
		get_full_section_name(full_name);
		full_keyname_ = full_name;

		file_index_ = 0;
		orig_index_ = 0;
	}

	~cfgunit_ini()
	{
		value_set_.clear();
		child_set_.clear();
	}

//接口函数
	const char *name() const
	{
		return keyname_.c_str();
	}

	const char* full_name() const
	{
		return full_keyname_.c_str();
	}

	int get_value(const char *value_name,const char*& pp_value,const char *subkey = 0) const
	{
		cfgunit *temp_node = get_sub_key(subkey);
		if(!temp_node)
		{
			return CONFIG_ERR_COMMON;
		}

		if(subkey)
		{
			return temp_node->get_value(value_name,pp_value);
		}
		else
		{
			config_enum_data enum_data;
			strcpy(enum_data.value_name,value_name);

			std::set<config_enum_data>::const_iterator p =  value_set_.find(enum_data);

			if(p == value_set_.end())
			{
				return CONFIG_ERR_COMMON;
			}

			pp_value = (*p).data_value.c_str();
		}

		return CONFIG_ERR_OK;
	}

	//读写操作
	int get_value(const char *value_name,unsigned long& value,const char* subkey = 0) const
	{
		const char *value_str = NULL;
		int ret = get_value(value_name,value_str,subkey);
		if(ret != CONFIG_ERR_OK)
		{
			return ret;
		}

		value = atoi(value_str);

		if(value == 0 && strstr(value_str,"0") == NULL)
		{
			return CONFIG_ERR_COMMON;
		}

		return CONFIG_ERR_OK;
	}

	int set_value(const char *value_name,unsigned long value,const char* subkey = 0)
	{
// 		char value_str[255] = { 0 };
// 		itoa(value,value_str,10);
		std::ostringstream oss;
		oss << value;
		int ret = set_value(type_string, value_name, oss.str().c_str(), subkey);
		return ret;
	}

	int set_value(const char *value_name,const char *value,const char* subkey = 0)
	{
		int ret = set_value(type_string,value_name,value,subkey);
		return ret;
	}

	int get_subunit_count(const char* subkey = 0) const
	{
		cfgunit *temp_node = get_sub_key(subkey);
		if(!temp_node)
		{
			return 0;
		}

		if(subkey)
		{
			return temp_node->get_subunit_count(subkey);
		}
		else
		{
			return (unsigned int)( child_set_.size() + link_key_set_.size());
		}

	}

	cfgunit* get_subunit_at(int index,const char* subkey = 0) const
	{
		cfgunit *temp_node = get_sub_key(subkey);
		if(!temp_node)
		{
			return NULL;
		}

		if(subkey)
		{
			return temp_node->get_subunit_at(index,subkey);
		}
		else
		{
			if(index < 0 || index > get_subunit_count() - 1)
				return NULL;

			if(index < (int)child_set_.size())
			{
				std::set<cfgunit_ini>::const_iterator p = child_set_.begin();
				int i = 0;
				for(;i<index;i++)
				{
					p++;
				}

				return (cfgunit* )&(*p);
			}
			else
			{
				//add by zwzhao 2006-07-17,如果没有找到，就到link下面去找
				std::map<std::string,std::string>::const_iterator p = link_key_set_.begin();
				int link_index = index - (int)child_set_.size();
				int i = 0;

				cfgunit *link_unit = get_root_node()->find_subunit(p->second.c_str());

				for(;i<link_index;p++,i++)
				{
					//先找到link对应键值的node
					link_unit = get_root_node()->find_subunit(p->second.c_str());
					SP_ASSERT(link_unit && "键都没有,你叫我怎么link?");
				}

				return link_unit;
			}


/*
			if(index <= current_key_index_)
			{
				current_key_index_ = 0;
				current_key_iterator = child_set_.begin();
			}

			for(;current_key_index_ < index;current_key_index_++)
			{
				current_key_iterator++;
			}

			return (cfgunit* )&(*current_key_iterator);
*/
		}

	}

	cfgunit *get_sub_key(const char *subkey = 0) const
	{
		if(!subkey)
		{
			return (cfgunit *)this;
		}
		else
		{
			return find_subunit(subkey);
		}
	}

	int get_values_count(const char* subkey = 0) const
	{
		cfgunit *temp_node = get_sub_key(subkey);
		if(!temp_node)
		{
			return 0;
		}

		if(subkey)
		{
			return temp_node->get_values_count();
		}
		else
		{
			return (unsigned int)value_set_.size();
		}
	}

	const char *get_value_at(int index,const char* subkey = 0) const
	{
		cfgunit *temp_node = get_sub_key(subkey);
		if(!temp_node)
		{
			return NULL;
		}

		if(subkey)
		{
			return temp_node->get_value_at(index);
		}
		else
		{
			if(index < 0 || index > (int)(value_set_.size()))
			{
				return NULL;
			}

			int i = 0;
			
			std::set<config_enum_data>::const_iterator p = value_set_.begin();
			for(;i<index;i++)
			{
				p++;
			}

			return (*p).value_name;

		}

	}

	cfgunit *get_parent() const
	{
		return parent_node_;
	}

	int append_sub(const char*key_name,cfgunit** pp_sub = NULL,const char* subkey = 0)
	{
		cfgunit *temp_node = get_sub_key(subkey);
		if(!temp_node)
		{
			return CONFIG_ERR_COMMON;
		}

		if(subkey)
		{
			return temp_node->append_sub(key_name,pp_sub);
		}
		else
		{
			cfgunit_ini addkey(key_name,this);
			*pp_sub = add_child(addkey);
			return CONFIG_ERR_OK;
		}

//		return CONFIG_ERR_OK;

	}

	int remove_sub(const char *key_name,const char *subkey = 0)
	{
		cfgunit *temp_node = get_sub_key(subkey);
		if(!temp_node)
		{
			return CONFIG_ERR_COMMON;
		}

		if(subkey)
		{
			return temp_node->remove_sub(key_name);
		}
		else
		{
			cfgunit_ini delkey(key_name,this);

			if(child_set_.find(delkey) == child_set_.end())
			{
				return CONFIG_ERR_COMMON;
			}
			else
			{
				child_set_.erase(delkey);
			}
		}
		return CONFIG_ERR_OK;
	}

	int remove_value(const char *value_name,const char *subkey = 0)
	{
		cfgunit *temp_node = get_sub_key(subkey);
		if(!temp_node)
		{
			return CONFIG_ERR_COMMON;
		}

		if(subkey)
		{
			return temp_node->remove_value(value_name);
		}
		else
		{
			config_enum_data enum_data;
			strcpy(enum_data.value_name,value_name);

			if(value_set_.find(enum_data) == value_set_.end())
			{
				return CONFIG_ERR_COMMON;
			}
			else
			{
				value_set_.erase(enum_data);
			}
		}
		return CONFIG_ERR_OK;
	}

	bool is_root_node()
	{
		if(get_parent() == NULL)
			return true;
		else
			return false;
	}

	const cfgunit *get_root_node() const
	{
		const cfgunit *root_node = this;
		while(root_node->get_parent())
		{
			root_node = root_node->get_parent();
		}

		return root_node;
	}

	void get_full_section_name(char *full_section_name)
	{
		std::string temp_str = name();

		cfgunit *temp_node = this->get_parent();
		if(temp_node == NULL)
		{
			strcpy(full_section_name,temp_str.c_str());
			return;
		}

		while(temp_node->get_parent() != NULL)
		{
			temp_str = "\\" + temp_str;
			temp_str = temp_node->name() + temp_str;
			temp_node = temp_node->get_parent();
		}
		strcpy(full_section_name,temp_str.c_str());
		assert(strlen(full_section_name) < 255);
	}

	bool operator < (const cfgunit_ini &p1) const
	{
		return stricmp(keyname_.c_str() , p1.name()) < 0;
	}

	bool operator==(const cfgunit_ini &p1) const
	{
		if(stricmp(keyname_.c_str() , p1.name()) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static bool less_by_index(const cfgunit_ini &p1, const cfgunit_ini &p2)
	{
		return p1.get_orig_index() < p2.get_orig_index();
	}

	bool is_quanjiao(const char* pstr)
	{
		SP_ASSERT(pstr);
		if ( pstr == 0 )
			return false;
		if ( *pstr == 0 || *(pstr+1) == 0 )
			return false;
		return ((*pstr) & 0x80) && ((*pstr) != 0xFF);
	}

	int trim_str(char* pstr, const char trim_char = ' ' )
	{
		const char* p = pstr;
		if ( 0 == p )
			return 0;
		// Get start and end position
		int start = 0, end = 0;
		while ( *p )
		{
			// 全角
			if ( is_quanjiao(p) )
			{
				if ( *((unsigned short *)p) == 0xa1a1 )
				{
					if ( end == 0 )
						start += 2;
				}
				else
				{
					end = (int) (p - pstr + 2);
				}
				p += 2;
				continue;
			}
			// 半角
			if ( ( (unsigned char)*p < 0x20 || trim_char == *p ) )
			{
				if ( end == 0 )
					start ++;
			}
			else
				end = (int) (p - pstr + 1);
			p++;
		}
		// trim it
		end > 0 ? pstr[end] = 0 : end = (int) (p - pstr);
		if ( end == start )
			pstr[0] = 0;
		else if ( start > 0 )
			memmove(pstr, pstr+start, end - start + sizeof(char));

		return end - start;
	}


	int parse_value_str(const char*value_str,config_enum_data &config_data)
	{
		int ret = CONFIG_ERR_OK;

		//判断是否还有引号，如果有，就是string
		const char *p1 = strstr(value_str,"=");
		assert(p1);

		strncpy(config_data.value_name,value_str,p1 - value_str);

		trim_str(config_data.value_name);

		config_data.data_type = type_string;
		//指向第一个引号
		p1 = p1+1;
		//指向第二个引号
		char *ptemp = new char[strlen(p1)+1];
		strcpy(ptemp,p1);
		trim_str(ptemp);
		config_data.data_value = ptemp;

		delete[] ptemp;
		ptemp = NULL;
		return ret;

	}

	int insert_value(const char *value_str,int file_index)
	{
		config_enum_data config_data;
		config_data.file_index = file_index;
		config_data.orig_index = (int)(value_set_.size() - 1);

		int ret = parse_value_str(value_str,config_data);

		if(config_data.value_name[0]  == '$')
		{
			link_key_set_.insert(std::make_pair<std::string,std::string>(config_data.value_name+1,config_data.data_value));
		}
		else
		{
			//如果有重复的，后面的冲掉前面的
			std::pair<std::set<config_enum_data>::iterator,bool> p = value_set_.insert(config_data);

			if(!p.second)
			{
				config_enum_data *temp_value = (config_enum_data *)&(*(p.first));
				temp_value->data_value = config_data.data_value;
			}
		}
		return ret;
	}

	//设置key的名称
	void set_keyname(const char *key_name)
	{
		keyname_ = key_name;

		char full_name[256] = { 0 };
		get_full_section_name(full_name);
		full_keyname_ = full_name;
	}

	//添加一个值项
	int add_value(const config_enum_data config_data,int file_index = -1)
	{
		std::pair<std::set<config_enum_data>::iterator,bool> p = value_set_.insert(config_data);

		if(p.second == true)
		{
			config_enum_data *temp_value = (config_enum_data *)&(*(p.first));

			if(file_index == -1)
			{
				temp_value->file_index = get_file_index();
			}
			else
			{
				temp_value->file_index = file_index;
			}
			temp_value->orig_index = (int)(value_set_.size()-1);
		}
		return (int)p.second;
	}

	//根据section_name创建node,如果已经有了，直接返回已有的node
	cfgunit_ini *create_node(const char *section_name,int file_index)
	{
		assert(section_name[0] == '[');

		char raw_section_name[255] = { 0 };
		const char *p = strstr(section_name,"]");
		assert(p);

		strncpy(raw_section_name,section_name+1,(p-section_name)-1);

		const char *pfirst = raw_section_name;
		const char *psecond = strstr(pfirst,"\\");
		char key_name[255] = { 0 };
		cfgunit_ini *temp_node = this;

		while(psecond)
		{
			strncpy(key_name,pfirst,(psecond - pfirst));

			cfgunit_ini childnode(key_name,temp_node);
			temp_node = temp_node->add_child(childnode,file_index);

			memset(key_name,0,255);

			pfirst = psecond + 1;
			psecond = strstr(pfirst,"\\");
		}

		strcpy(key_name,pfirst);

		cfgunit_ini childnode(key_name,temp_node);
		temp_node = temp_node->add_child(childnode,file_index);

		return temp_node;
	}

	//在当前节点下寻找指定key_name的子节点，找不到就返回NULL
	cfgunit_ini *find_child(const char *key_name)
	{
		assert(key_name);

		cfgunit_ini temp_node(key_name,this);
		std::set<cfgunit_ini>::iterator p = child_set_.find(temp_node);
		if(p == child_set_.end())
		{
			//add by zwzhao 2006-07-17,如果没有找到，就到link下面去找
			std::map<std::string,std::string>::const_iterator p = link_key_set_.begin();
			for(;p!=link_key_set_.end();p++)
			{
				//先找到link对应键值的node
				if(p->first == key_name)
				{
					cfgunit_ini *link_unit = static_cast<cfgunit_ini*>(get_root_node()->find_subunit(p->second.c_str()));
					SP_ASSERTS( link_unit , ("键 %s 都没有，你叫我怎么link?",p->second.c_str()));

					return link_unit;
				}
			}

			return NULL;
		}
		else
		{
			return (cfgunit_ini *)&(*p);
		}
	}

	//根据section_name找node,没找到就返回NULL
	cfgunit *find_subunit(const char *section_name) const
	{
		const char *pfirst = section_name;
		const char *psecond = strstr(pfirst,"\\");
		char key_name[255] = { 0 };
		cfgunit_ini *temp_node = (cfgunit_ini *)this;

		while(psecond)
		{
			strncpy(key_name,pfirst,(psecond - pfirst));
			temp_node = temp_node->find_child(key_name);
			if(temp_node == NULL)
			{
				return NULL;
			}

			memset(key_name,0,255);

			pfirst = psecond + 1;
			psecond = strstr(pfirst,"\\");
		}

		strcpy(key_name,pfirst);
		temp_node = temp_node->find_child(key_name);
		if(temp_node == NULL)
		{
			//add by zwzhao 2006-07-17,如果没有找到，就到link下面去找
			std::map<std::string,std::string>::const_iterator p = link_key_set_.begin();
			for(;p!=link_key_set_.end();p++)
			{
				//先找到link对应键值的node
				if(p->first == key_name)
				{
					cfgunit *link_unit = get_root_node()->find_subunit(p->second.c_str());
					SP_ASSERTS( link_unit , ("键 %s 都没有，你叫我怎么link?",p->second.c_str()));

					return link_unit;
				}
			}
		}
		return temp_node;
	}

	//加入一个子节点，并且返回加入的字节点
	cfgunit_ini *add_child(const cfgunit_ini &childnode,int file_index = -1)
	{
		std::pair<std::set<cfgunit_ini>::iterator,bool> p = child_set_.insert(childnode);

		if(p.second == false)
		{
			return (cfgunit_ini *)&(*(p.first));
		}
		else
		{
			cfgunit_ini *temp_node = (cfgunit_ini *)&(*(p.first));
			temp_node->set_parent_node(this);

			if(file_index_ == -1)
			{
				temp_node->set_file_index(this->get_file_index());
			}
			else
			{
				temp_node->set_file_index(file_index);
			}
			temp_node->set_orig_index((int)(child_set_.size()-1));

			return (cfgunit_ini *)&(*(p.first));
		}
	}

	int set_value(config_data_type data_type,const char *value_name,const char *value_str,const char *subkey = 0)
	{
		assert(data_type == type_string);

		cfgunit_ini *temp_node = (cfgunit_ini *)get_sub_key(subkey);
		if(!temp_node)
		{
			return CONFIG_ERR_COMMON;
		}

		if(subkey)
		{
			return temp_node->set_value(data_type,value_name,value_str,NULL);
		}
		else
		{
			config_enum_data enum_data;
			strcpy(enum_data.value_name,value_name);

			std::set<config_enum_data>::iterator p =  value_set_.find(enum_data);
			if(p != value_set_.end())
			{
				// p->data_value = value_str;
				enum_data = *p;
				enum_data.data_value = value_str;
				value_set_.erase(p);
				value_set_.insert(enum_data);
				return CONFIG_ERR_OK;
			}
			else
			{
				enum_data.data_type = data_type;
				enum_data.file_index = 0;
				enum_data.orig_index = 0;

				switch(data_type)
				{
				case type_string:
					enum_data.data_value = value_str;
					break;

				case type_dword:
					assert(0);
//					enum_data.value.nvalue = *((int*)value);
					break;
				case type_bool:
					assert(0);
//					enum_data.value.nvalue = *((bool*)value);
					break;

				default:
					SP_ASSERT(0 && "没找到相应的类型！");
				}

				value_set_.insert(enum_data);
			}

		}

		return CONFIG_ERR_OK;
	}


	int save(std::vector<std::ofstream *> &os_vector,const char *subkey = 0)
	{
		std::ostream &os = *os_vector[get_file_index()];

		std::vector<int> write_key;
		write_key.resize(os_vector.size());
		for(unsigned int i=0;i<write_key.size();i++)
			write_key[i] = 0;


		cfgunit_ini *temp_node = (cfgunit_ini *)get_sub_key(subkey);
		if(!temp_node)
		{
			return CONFIG_ERR_COMMON;
		}

		if(subkey)
		{
			return temp_node->save(os_vector,subkey);
		}
		else
		{
			if(!is_root_node())
			{
				//输出值项
				//按照原始的顺序

				std::vector<config_enum_data> value_value_sortbyindex;
				value_value_sortbyindex.resize(value_set_.size());

				std::copy(value_set_.begin(),value_set_.end(),value_value_sortbyindex.begin());

				std::sort(value_value_sortbyindex.begin(),
											value_value_sortbyindex.end(),
											config_enum_data::less_by_index);

				std::vector<config_enum_data>::iterator pdata = value_value_sortbyindex.begin();
				for(;pdata != value_value_sortbyindex.end();pdata++)
				{
					//输出键值,只有在有值项的时候，才输出键值
					if(write_key[pdata->file_index] ==  0)
					{
						char szfullname[255] = { 0 };
						get_full_section_name(szfullname);
						*os_vector[pdata->file_index] << std::endl;
						*os_vector[pdata->file_index] << "[" << szfullname << "]" << std::endl;

						write_key[pdata->file_index] = 1;
					}

					char value_str[1024] = { 0 };
					cfgunit_utility::create_value_str(*pdata,value_str);
					*os_vector[pdata->file_index] << value_str << std::endl;
				}

				//写链接
				if(value_value_sortbyindex.size()!=0)
				{
					std::map<std::string,std::string>::iterator link_p = link_key_set_.begin();
					for(;link_p!=link_key_set_.end();link_p++)
					{
						*os_vector[value_value_sortbyindex.begin()->file_index] << "$" << link_p->first.c_str() << "=" << link_p->second.c_str() << std::endl;
					}
				}
			}

			//递归调用
			std::vector<cfgunit_ini> node_vector_sortbyindex;
			node_vector_sortbyindex.resize(child_set_.size());
			std::copy(child_set_.begin(),child_set_.end(),node_vector_sortbyindex.begin());

			std::sort(node_vector_sortbyindex.begin(),
									node_vector_sortbyindex.end(),
									cfgunit_ini::less_by_index);

			std::vector<cfgunit_ini>::iterator pchild = node_vector_sortbyindex.begin();
			for(;pchild!=node_vector_sortbyindex.end();pchild++)
			{
				cfgunit_ini *temp_node = (cfgunit_ini *)&(*pchild);
				temp_node->save(os_vector);
			}
		}


//		os.flush();


		return CONFIG_ERR_OK;

	}

	void set_parent_node(cfgunit_ini *parent_node)
	{
		parent_node_ = parent_node;
	}

	int load(const char *holder_name,int file_index)
	{
		int ret = CONFIG_ERR_OK;
		cfgunit_ini *current_node = NULL;
		FILE *f = fopen(holder_name,"rt");
		if(!f)
		{
			assert(0 && "file not found!");
			return CONFIG_ERR_COMMON;
		}

		bool is_first_line = true;

		//逐步，每行的加载
		while(!feof(f))
		{
			char szline[255] = { 0 };
			fgets(szline,255,f);

			if(is_first_line)
			{
				is_first_line = false;
			}

			//滤掉一些注释
			if(strlen(szline) == 0 
				|| (!is_key(szline) && !is_value(szline))
				|| szline[0] == ';' || szline[0] == '\\')
			{
				continue;
			}

			//将换行符滤掉
			if(szline[strlen(szline)-1] == '\n')
			{
				szline[strlen(szline)-1] = 0;
			}

			if(szline[strlen(szline)-1] == '\r')
			{
				szline[strlen(szline)-1] = 0;
			}

			//如果是键，则创建键值
			if(is_key(szline))
			{
				current_node = create_node(szline,file_index);
			}
			//如果是值，在当前键下创建项
			else if(is_value(szline))
			{
				//将"\\"换成"\"
				std::string new_line = szline;
				//				IConfig::string_replace(new_line,"\\\\","\\");

				current_node->insert_value(new_line.c_str(),file_index);
			}
		}
		fclose(f);

		return ret;

	}

	void set_orig_index(int index)
	{
		orig_index_ = index;
	}
		
	void set_file_index(int index)
	{
		file_index_ = index;
	}

	int get_orig_index() const
	{
		return orig_index_;
	}

	int get_file_index() const
	{
		return file_index_;
	}

	//zwzhao add following code,为了处理link的情况
	void proc_link_key()
	{
		
	}
	//add end

private:

	//判断当前行是不是键
	bool is_key(const char *szline) const
	{
		if(szline[0] == '[')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//判断当前行是不是值项
	bool is_value(const char *szline) const
	{
		if(strstr(szline,"=") != NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	std::string keyname_;	//Node 名称
	std::string full_keyname_;
	std::set<config_enum_data> value_set_;	//保存value的set
	std::set<cfgunit_ini>	child_set_;			//保存childnode的set
	std::map<std::string,std::string> link_key_set_;

	cfgunit_ini *parent_node_;

	//为了实现多个文件的回写加的功能 //add by zwzhao 2006-02-23
	int file_index_;	//是从第几个文件中读出来的
	int orig_index_;	//在原文件中的原始排列顺序
	//add end
};


class cfgunit_ini_root : public cfgunit_ini
{
public:
	cfgunit_ini_root(const char *keyname,cfgunit_ini *parent_node) : cfgunit_ini(keyname,parent_node)
	{

	}

	int save()
	{
		unsigned int i=0;

		unsigned int ret = CONFIG_ERR_OK;

		for(;i<filename_vector.size();i++)
		{
			std::ofstream *pos = new std::ofstream(filename_vector[i].c_str());
			filehandle_vector.push_back(pos);
		}

		ret = cfgunit_ini::save(filehandle_vector);

		for(i=0;i<filehandle_vector.size();i++)
		{
			filehandle_vector[i]->close();
			delete filehandle_vector[i];
			filehandle_vector[i] = NULL;
		}
		return ret;
	}

	void add_file(const char *file_name)
	{
		filename_vector.push_back(file_name);
	}

private:
	std::vector<std::string> filename_vector;
	std::vector<std::ofstream*> filehandle_vector;
};


class cfg_holder_ini
	: public cfg_holder
	, public scom::singleton<cfg_holder_ini>
	{
public:
	cfg_holder_ini()
		{
		root_node_ = NULL;
		file_count_ = 0;
		}

	virtual int open(const char* start_path = NULL,bool append = false, const char * cfg_dir = 0)
		{
		if (start_path != NULL)
			{
			installed_path_ = start_path;
			}
		else
			{
			SP_ASSERT(false && "libpath is empty!");
#ifdef  _DEBUG
			std::cout << "holder_name is empty!" << std::endl;
#endif
			return -1;
			}

		if (root_node_ && !append)
			{
			delete root_node_;
			root_node_ = NULL;
			file_count_ = 0;
			}

		if (!root_node_)
			{
			root_node_ = new cfgunit_ini_root("ROOT_NODE",NULL);
			}

		const char * cfg_dirname = _cfg_dirname;
		if ( cfg_dir )
			cfg_dirname = cfg_dir;

		//查找lib,以及lib\addins目录下的cfu
		trb::ospath cfu_in_lib	  = installed_path_ + cfg_dirname + "*.cfu";
		trb::ospath cfu_in_addins = installed_path_ + cfg_dirname + addin_dirname + "*.cfu";
		trb::ospath cfu_in_voclib = installed_path_ + cfg_dirname + "voclib" + "*.cfu";
		trb::ospath cfu_in_misc   = installed_path_ + cfg_dirname + "misc" + "*.cfu";

		std::vector<trb::ospath> v_all_cfu;
		cfu_in_lib.subst_wd(v_all_cfu);
		cfu_in_addins.subst_wd(v_all_cfu);
		cfu_in_voclib.subst_wd(v_all_cfu);
		cfu_in_misc.subst_wd(v_all_cfu);

		if(v_all_cfu.size() == 0)
			{
			return -1;
			}

		int ret = CONFIG_ERR_OK;

		//Added by zwzhao, 2006-1-18 14:21:53
		//添加循环遍历的功能
		std::vector<trb::ospath>::iterator p = v_all_cfu.begin();
		for(;p != v_all_cfu.end();p++)
			{
			int nret = root_node_->load((*p).c_str(),file_count_);
			if(nret == CONFIG_ERR_OK)
				{
				isloaded_ = true;
				current_file_ = p->c_str();

				root_node_->add_file(p->c_str());
				}
			file_count_++;
			}

		return ret;
		}

	virtual int flush()
		{
		root_node_->save();
		return 0;
		}

	virtual int destroy()
		{
		if ( root_node_ )
			delete root_node_, root_node_ = 0;
		return 0;
		}

	virtual cfgunit* get_cfgunit(const char* subkey = NULL) const
		{
		if(!subkey)
			{
			return (cfgunit*)root_node_;
			}
		else
			{
			return root_node_->find_subunit(subkey);
			}
		}

	virtual const char* get_install_path() const
		{
		return installed_path_.c_str();
		}

private:
	cfgunit_ini_root *root_node_;		//根节点

	bool isloaded_;				//配置文件是否已经加载
	std::string current_file_;	//当前文件名称

	int file_count_;
	trb::ospath installed_path_;
	};

DEFINE_SHIMP(cfg_holder, cfg_holder_ini, unique);

// 灭掉warning 4996
#if _MSC_VER > 1300
#  pragma warning(pop)
#endif /* _MSC_VER > 1300 */

#endif 
