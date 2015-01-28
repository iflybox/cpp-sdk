#ifndef	__OSPATH_INCLUDED
#define __OSPATH_INCLUDED

#include "oslayer.hpp"	// need os_directory.

namespace trb {

class pathstorage
	{
public:
	static const int path_max = 4096 - 2 * sizeof(int);
	void addref()
		{
		assert(path_obj_ != NULL);
		path_obj_->ref_count_ ++;
		}

	void release()
		{
		assert(path_obj_ != NULL);
		path_obj_->ref_count_ --;
		if (path_obj_->ref_count_ == 0)
			delete path_obj_;

		path_obj_ = NULL;
		}
	
	const char* c_str() const
		{
		if (path_obj_ == NULL)
			return NULL;
		return path_obj_->str_buff_;
		}

	size_t length() const
		{
		assert(path_obj_ != NULL);
		return path_obj_->str_length_;
		}

	const char* alloc(const char* str1 = NULL)
		{
		if (path_obj_ != NULL &&
			path_obj_->ref_count_ > 1)
			release();

		if (path_obj_ == NULL)
			{
			path_obj_ = new path_blk;
			path_obj_->ref_count_ = 1;
			path_obj_->str_length_ = 0;
			path_obj_->str_buff_[0] = 0;
			}
		return append(str1, 0);
		}

	const char* append(const char* str, int bpos = -1)
		{
		if (bpos == -1 
			&& (str == NULL || *str == 0))
			return c_str();		// 在结束处被空字符串，无意义。

		if (str == NULL)
			{
			str = "";
			}
		assert(path_obj_ != NULL && path_obj_->ref_count_ == 1);
		if (bpos >= 0)
			path_obj_->str_length_ = bpos;

		while (path_obj_->str_length_ < path_max-1 && *str)
			{
			path_obj_->str_buff_[path_obj_->str_length_++] = *str++;
			}
		path_obj_->str_buff_[path_obj_->str_length_] = 0;
		return path_obj_->str_buff_;
		}

	pathstorage()
		{
		path_obj_ = NULL;
		}

	pathstorage(const pathstorage& ps0)
		{
		attach_blk(ps0.path_obj_);
		}

	pathstorage& operator = (const pathstorage& ps0)
		{
		attach_blk(ps0.path_obj_);
		return *this;
		}

	~pathstorage()
		{
		if (path_obj_ != NULL)
			release();
		}

	bool inited() const
		{
		return path_obj_ != NULL;
		}

protected:
	struct	path_blk
		{
		volatile size_t	ref_count_;
		volatile size_t str_length_;
		char	str_buff_[path_max];
		};
	path_blk* path_obj_;

	void attach_blk(path_blk* pop)
		{
		path_obj_ = pop;
		if (path_obj_ != NULL)
			addref();
		}
	};

class ospath;
typedef std::vector<ospath>	ospvector;

/** 
 * @class		class ospath
 * 
 * @brief	
 * 
 * not thread-safe.
 * 
 * @author	wbli
 * @date	2005年11月2日
 * 
 * @see		
 * 
 * @par 备注：
 * 
 */
class ospath
	{
public:
#ifdef	WIN32
	static const int slash = '\\';
#else
	static const int slash = '/';
#endif // WIN32

	ospath()
		{
		path_string_ = NULL;
		}

	ospath(const ospath & p2)
		{
		assign(p2);
		}

	ospath(const char* path, bool copy=true)
		{
		assign(path, copy);
		}

	const char* c_str() const
		{
		return path_string_;
		}

	size_t length() const
		{
		return path_storage_.length();
		}

	bool empty() const
		{
		return path_string_ == NULL;
		}

	bool absolute() const
		{
		if (path_string_ == NULL || *path_string_ == 0)
			return false;	// false if empty

		if (path_string_[0] == '~' ||
			is_splitter(path_string_[0]) ||
			path_string_[1] == ':')
			return true;
		return false;
		}

	bool wildchar() const
		{
		if (path_string_ == NULL || *path_string_ == 0)
					return false;	// false if empty
		const char* ps = path_string_;
		while (*ps != 0 && *ps != '*' && *ps != '?')
			ps ++;
		return *ps != 0;
		}

	ospath& assign(const ospath& p0)
		{
		path_storage_ =  p0.storage();
		path_string_ = p0.c_str();
		return *this;
		}

	ospath& assign(const char* path, bool copy=true)
		{
		path_string_ = path;
		if (copy)
			{
			save();
			}
		return *this;
		}

	void save()
		{
		if (path_string_ == NULL ||
			path_string_ != path_storage_.c_str())
			{
			path_string_ = path_storage_.alloc(path_string_);
			}
		}

	void append(const char* p1)
		{
		path_string_ = path_storage_.alloc(path_string_);
		storage().append(p1);
		}

	void shorten(size_t nlen)
		{
		path_string_ = path_storage_.alloc(c_str());
		path_storage_.append("", (int)nlen);
		}

	ospath join (const ospath& p2) const
		{
		return join(p2.c_str());
		}

	ospath join(const char* p2) const
		{
		ospath p1 = *this;
		if (p2 == NULL || *p2 == 0)
			return p1;

		p1.save();
		// parse input string.
		const char* pbegin = p2;
		int skip_dir = 0;
		while (*pbegin == '.')
			{
			pbegin ++;
			if (*(pbegin) == '.')
				{
				pbegin ++;
				skip_dir ++;
				}
			if (is_splitter(*pbegin))
				pbegin ++;
			}

		int bpos = (int)p1.length()-1;
		while (bpos > 0 && skip_dir > 0)
			{
			int real = 0;
			while (bpos > 0 && !is_splitter(path_string_[bpos]))
				{
				real = 1;
				bpos --;
				}
			skip_dir -= real;
			bpos --;
			}

		assert(skip_dir == 0);
		p1.shorten(bpos+1);
		if ((bpos >= 0) && !is_splitter(path_string_[bpos]))
			{
			char ss[] = {slash, 0};
			p1.append(ss);
			}

		p1.append(pbegin);
		return p1;
		}

	ospath extract_dir() const
		{
		ospath sdir(c_str());
		const char* fname = extract_name();
		size_t len = fname - c_str();
		sdir.shorten(len);
		return sdir;
		}

	ospath subst_ext(const char* new_ext) const
		{
		ospath newp(c_str());
		int t = (int)newp.length();
		int i;
		for (i = t-1; i >= 0; i --)
			{
			if (is_splitter(path_string_[i]))
				{
				i = -1;
				break;
				}
			if (path_string_[i] == '.')
				{
				break;
				}
			}
		if (i >= 0)
			{
			newp.shorten(i);
			newp.storage().append(new_ext);
			}
		return newp;
		}

	const char* extract_name() const
		{
		int i;
		int last_sp = -1;
		for (i = 0; path_string_[i] != 0; i ++)
			{
			if (is_splitter(path_string_[i]))
				{
				last_sp = i;
				}
			}
		return path_string_+last_sp+1;
		}

	ospath to_abs() const
		{
		if (absolute())
			return *this;

		return ospath(cwd_string().c_str(), false) + *this;
		}

	void subst_wd(ospvector& dir_v) const
		{
		os_directory osd;
		ospath dirp = extract_dir();
		if (0 != osd.open(dirp.c_str()))
			return;

		const char* fname;
		bool cont;
		while ((fname = osd.next(cont)) != NULL)
			{
			ospath ona = dirp.join(fname);
			if (match(ona.c_str()))
				dir_v.push_back(trb::ospath(ona.c_str()));
			if (!cont)
				break;
			}

		osd.close();
		}

	bool match(const char* p2) const
		{
		ospath pa2(p2, false);
		// if this is not absolute path, but param is absolute, failed.
		// please call to_abs first.
		trb_assert(absolute() || !pa2.absolute());
		if (absolute() != pa2.absolute())
			{
			pa2 = pa2.to_abs();
			}
		return patt_match(c_str(), pa2.c_str());
		}

	// convert to relative path form according to p2
	ospath relative_to(const char* p2) const
		{
		ospath curp(p2, false);
		curp = curp.to_abs();
		ospath absp = to_abs();
		// declarations - put here so this should work in a C compiler
		size_t cdLen = 0, afLen = 0;
		size_t afMarker = 0, rfMarker = 0;
		size_t i = 0;
		size_t levels = 0;
//		static char relativeFilename[MAX_FILENAME_LEN+1];

		cdLen = curp.length(); // strlen(curp);
		afLen = absp.length(); // strlen(absp);

// 		// make sure the names are not too long or too short
// 		if(cdLen > pathstorage::path_max || cdLen < ABSOLUTE_NAME_START+1 ||
// 			afLen > MAX_FILENAME_LEN || afLen < ABSOLUTE_NAME_START+1)
// 		{
// 			return NULL;
// 		}

// 		// Handle DOS names that are on different drives:
// 		if(curp[0] != absp[0])
// 		{
// 			// not on the same drive, so only absolute filename will do
// 			strcpy(relativeFilename, absp);
// 			return relativeFilename;
// 		}
		if (curp[0] != absp[0])
			return absp;

		// they are on the same drive, find out how much of the current directory
		// is in the absolute filename
//		i = ABSOLUTE_NAME_START;
		i = 0;
		while(i < afLen && i < cdLen && curp[i] == absp[i])
			{
			i++;
			}

		if(i == cdLen && (is_splitter(absp[i])|| is_splitter(absp[i-1])))
			{
// 			// the whole current directory name is in the file name,
// 			// so we just trim off the current directory name to get the
// 			// current file name.
// 			if(absp[i] == SLASH)
// 			{
// 				// a directory name might have a trailing slash but a relative
// 				// file name should not have a leading one...
// 				i++;
// 			}
// 
// 			strcpy(relativeFilename, &absp[i]);
// 			return relativeFilename;
			return ospath(absp.extract_name(), false);
			}


		// The file is not in a child directory of the current directory, so we
		// need to step back the appropriate number of parent directories by
		// using "..\"s.  First find out how many levels deeper we are than the
		// common directory
		afMarker = i;
		levels = 1;

		// count the number of directory levels we have to go up to get to the
		// common directory
		while(i < cdLen)
			{
			i++;
			if(is_splitter(curp[i]))
				{
				// make sure it's not a trailing slash
				i++;
				if(curp.c_str()[i] != '\0')
					{
					levels++;
					}
				}
			}

		// move the absolute filename marker back to the start of the directory name
		// that it has stopped in.
		while(afMarker > 0 && is_splitter(absp[afMarker-1]))
			{
			afMarker--;
			}

		// check that the result will not be too long
		if(levels * 3 + afLen - afMarker > pathstorage::path_max)
			{
			return ospath();
			}

		ospath rel_path;
		// add the appropriate number of "..\"s.
		rfMarker = 0;
		for(i = 0; i < levels; i++)
			{
			rel_path += "..";
			rel_path += (char)slash;
// 			relativeFilename[rfMarker++] = '.';
// 			relativeFilename[rfMarker++] = '.';
// 			relativeFilename[rfMarker++] = SLASH;
			}

		// copy the rest of the filename into the result string
		rel_path += absp.c_str() + afMarker;
//		strcpy(&relativeFilename[rfMarker], &absp[afMarker]);

		return rel_path;
		}

	char operator[] (size_t opos) const
		{
		if (path_string_ == NULL)
			return 0;

		if (path_storage_.inited() && opos >= path_storage_.length())
			return 0;
			
		return path_string_[opos];
		}

	ospath operator + (const char* p2) const
		{
		return join(p2);
		}

	ospath operator + (const ospath& p2) const
		{
		return join(p2);
		}

	ospath& operator = (const ospath& p0)
		{
		return assign(p0);
		}

	ospath& operator = (const char* str)
		{
		return assign(str);
		}

	ospath& operator = (const std::string& str)
		{
		return assign(str.c_str());
		}

	ospath& operator += (const char* str)
		{
		return assign(join(str));
		}

	ospath& operator += (const ospath& p0)
		{
		return assign(join(p0));
		}

	ospath& operator += (char pc)
		{
		char cc[2] = {pc, 0};
		return assign(join(cc));
		}

	bool operator < (const trb::ospath &p1) const
	{
		return strcmp(path_string_,p1.c_str()) < 0;
	}

	static bool is_splitter(char sp)
		{
		return sp == '\\' || sp == '/';
		}

	static bool patt_match(const char* pattern, const char* path2)
		{
		if (!pattern || !path2)
			return false;

		char psbackup[pathstorage::path_max];
		size_t patt_len = strlen(pattern);
		if (patt_len >= pathstorage::path_max)
			patt_len = pathstorage::path_max-1;
		strncpy(psbackup, pattern, patt_len);
		psbackup[patt_len] = 0;

		// char* psbackup = strdup(pattern);
		char* ps1 = psbackup;
		const char *ps2 = path2;
		int succ = -1;

		while ((*ps1 != '\0') && (ps2 != '\0'))
			{
			switch (*ps1)
				{
			case '?':
				ps1++;
				ps2++;
				break;

			case '*':{
					// find existence of next block
					if (0 == *++ps1)
						{
						succ = 1;
						break;
						}
					char *pS = strchr (ps1, '*');
					char *pQ = strchr (ps1, '?');
					if (pS) {
						*pS = '\0';
					}
					if (pQ) {
						*pQ = '\0';
					}
					char *pStr = (char*)strstr (ps2, ps1);
					if (pS) {
						*pS = '*';
					}
					if (pQ) {
						*pQ = '?';
					}
					if (pStr) {
						ps2 = pStr;
						succ = patt_match (ps1, ps2);
						break;
					}
					succ = 0;
					break;	}

			default:
				if (*ps1 != *ps2)
					{
					succ = 0;
					break;
					}
				ps1++;
				ps2++;
				break;
				}
			if (succ != -1)
				break;
			}

		if (succ == -1)
			{
			if (((*ps1 == '\0') && (*ps2 == '\0')))
				succ = 1;
			else
				succ = 0;
			}

		return succ != 0;
		}

	static ospath get_cwd()
		{
		return ospath().to_abs();
		}

	static void set_cwd(const char* new_cwd)
		{
		if (new_cwd)
			cwd_string() = new_cwd;
		}

protected:
	static std::string& cwd_string()
		{	
		static std::string str_cwd(os_directory::get_current_dir());
		return str_cwd;
		}

	pathstorage& storage()
		{
		return path_storage_;
		}

	const pathstorage& storage() const
		{
		return path_storage_;
		}

private:
	const char*		path_string_;
	pathstorage		path_storage_;
	};

}	// end of namespace trb


#endif // __OSPATH_INCLUDED


//// GetRelativeFilename(), by Rob Fisher.
//// rfisher@iee.org
//// http://come.to/robfisher
//
//// includes
//#include <string.h>
//
//// defines
//#define MAX_FILENAME_LEN 512
//
//// The number of characters at the start of an absolute filename.  e.g. in DOS,
//// absolute filenames start with "X:\" so this value should be 3, in UNIX they start
//// with "\" so this value should be 1.
//#define ABSOLUTE_NAME_START 3
//
//// set this to '\\' for DOS or '/' for UNIX
//#define SLASH '\\'
//
//
//// Given the absolute current directory and an absolute file name, returns a relative file name.
//// For example, if the current directory is C:\foo\bar and the filename C:\foo\whee\text.txt is given,
//// GetRelativeFilename will return ..\whee\text.txt.
//
//char* GetRelativeFilename(char *curp, char *absp)
//{
//	// declarations - put here so this should work in a C compiler
//	int afMarker = 0, rfMarker = 0;
//	int cdLen = 0, afLen = 0;
//	int i = 0;
//	int levels = 0;
//	static char relativeFilename[MAX_FILENAME_LEN+1];
//
//	cdLen = strlen(curp);
//	afLen = strlen(absp);
//
//	// make sure the names are not too long or too short
//	if(cdLen > MAX_FILENAME_LEN || cdLen < ABSOLUTE_NAME_START+1 ||
//		afLen > MAX_FILENAME_LEN || afLen < ABSOLUTE_NAME_START+1)
//	{
//		return NULL;
//	}
//
//	// Handle DOS names that are on different drives:
//	if(curp[0] != absp[0])
//	{
//		// not on the same drive, so only absolute filename will do
//		strcpy(relativeFilename, absp);
//		return relativeFilename;
//	}
//
//	// they are on the same drive, find out how much of the current directory
//	// is in the absolute filename
//	i = ABSOLUTE_NAME_START;
//	while(i < afLen && i < cdLen && curp[i] == absp[i])
//	{
//		i++;
//	}
//
//	if(i == cdLen && (absp[i] == SLASH || absp[i-1] == SLASH))
//	{
//		// the whole current directory name is in the file name,
//		// so we just trim off the current directory name to get the
//		// current file name.
//		if(absp[i] == SLASH)
//		{
//			// a directory name might have a trailing slash but a relative
//			// file name should not have a leading one...
//			i++;
//		}
//
//		strcpy(relativeFilename, &absp[i]);
//		return relativeFilename;
//	}
//
//
//	// The file is not in a child directory of the current directory, so we
//	// need to step back the appropriate number of parent directories by
//	// using "..\"s.  First find out how many levels deeper we are than the
//	// common directory
//	afMarker = i;
//	levels = 1;
//
//	// count the number of directory levels we have to go up to get to the
//	// common directory
//	while(i < cdLen)
//	{
//		i++;
//		if(curp[i] == SLASH)
//		{
//			// make sure it's not a trailing slash
//			i++;
//			if(curp[i] != '\0')
//			{
//				levels++;
//			}
//		}
//	}
//
//	// move the absolute filename marker back to the start of the directory name
//	// that it has stopped in.
//	while(afMarker > 0 && absp[afMarker-1] != SLASH)
//	{
//		afMarker--;
//	}
//
//	// check that the result will not be too long
//	if(levels * 3 + afLen - afMarker > MAX_FILENAME_LEN)
//	{
//		return NULL;
//	}
//
//	// add the appropriate number of "..\"s.
//	rfMarker = 0;
//	for(i = 0; i < levels; i++)
//	{
//		relativeFilename[rfMarker++] = '.';
//		relativeFilename[rfMarker++] = '.';
//		relativeFilename[rfMarker++] = SLASH;
//	}
//
//	// copy the rest of the filename into the result string
//	strcpy(&relativeFilename[rfMarker], &absp[afMarker]);
//
//	return relativeFilename;
//}
