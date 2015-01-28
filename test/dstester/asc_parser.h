/** 
 * @file	asc_parser.h
 * @brief	自动化脚本解析器的头文件
 * 
 *  这个解析器支持比较简单的脚本文件格式为：
 *	# iFly Speech Recognizer Script file
 *	ISRrecInitialize
 *	ISRrecRecognizerCreate name	
 *	ISRrecGrammarLoad 
 *	ISRrecRecognizerDestroy
 *	ISRrecUninitialize
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2006-10-8
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2006-10-8	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __ASC_PARSER_H__
#define __ASC_PARSER_H__

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
//#include "iFly_Assert.h"

#ifndef WIN32//linux yanxu2 add 2010-12-29
# include <string.h>
# include <stdlib.h>
#endif

#include "utils/sutils.h"
#include "utils/sulib.h"
#include "log/dslog.h"
#ifndef WIN32
# include <unistd.h>
#endif // no win32

// Sun OS pack
#ifndef WIN32
#pragma pack(push)
#pragma pack(8)
#endif // sun

// namespace of acs_parser
namespace sc {

/**
 * 参数中属性对的定义
 */
template <typename char_t = char>
class asc_par_attr_t
	: public std::pair<std::basic_string<char_t>, std::basic_string<char_t> >
	{
public:
	typedef std::pair<std::basic_string<char_t>, std::basic_string<char_t> > parent;
	typedef std::basic_string<char_t> string;

	const string & par_name(void) const
		{
		return this->first;
		}
	string & par_name(void)
		{
		return this->first;
		}
	const string & par_value(void) const
		{
		return this->second;
		}
	string & par_value(void)
		{
		return this->second;
		}
	int int_value(void) const
		{
		if ( is_empty() || is_null() )
			return 0;
		return sp::strtoi(par_value().c_str());
		}
	unsigned int uint_value(void) const
		{
		if ( is_empty() || is_null() )
			return 0;
		return sp::strtoui(par_value().c_str());
		}
	double double_value(void) const
		{
		if ( is_empty() || is_null() )
			return 0.;
		return sp::strtod(par_value().c_str());
		}
	bool no_name(void) const
		{
		return no_name_;
		}
	bool is_empty(void) const
		{
		return this->first.size() == 0 && this->second.size() == 0;
		}
	bool is_null(void) const
		{
		static const char_t null_str[] = { 'n', 'u', 'l', 'l', '\0' };
		return sp::strcasecmp(par_value().c_str(), null_str) == 0;
		}
	bool is_zero(void) const
		{
		static const char_t zero_str = { '0', '\0' };
		return par_value() == zero_str;
		}
	bool operator == (const asc_par_attr_t & par) const
		{
		return sp::strcasecmp(par_name().c_str(), par.par_name().c_str()) == 0;
		}
	bool operator == (const string & parn) const
		{
		return sp::strcasecmp(par_name().c_str(), parn.c_str()) == 0;
		}

	asc_par_attr_t(const string & par_str)
		: no_name_	(false)
		{
		string pnm, val;
		const char_t avsplt[] = { '=', '\0' };

		sp::get_attrib_and_value(par_str.c_str(), pnm, val, avsplt, true);
		if ( par_str.find(avsplt[0]) == string::npos )
			{		
			no_name_ = true;
			this->second = pnm;
			}
		else
			{
			this->first  = pnm;
			this->second = val;
			}
		}
	asc_par_attr_t(void)
		: no_name_	(false)
		{
		}

private:
	bool no_name_;
	};

/**
 * 参数类，包含多个属性对
 */
template <typename char_t = char>
class asc_par_t
	: public std::vector<asc_par_attr_t<char_t> >
	{
public:
	typedef asc_par_attr_t<char_t>		par_attr;
	typedef std::basic_string<char_t>	string;
	typedef std::vector<string>			str_arr;

	typedef typename std::vector<asc_par_attr_t<char_t> >::iterator iterator;
	typedef typename std::vector<asc_par_attr_t<char_t> >::const_iterator const_iterator;

	asc_par_t(const string & par_str)
		{
		str_arr pars;
		const char_t par_splt[] = { ' ', '\t', '\0' };

		sp::split_str(par_str.c_str(), pars, par_splt, true, false, true);
		for ( int i = 0; i < (int)pars.size(); i++ )
			{
			// 参数值需要支持类似"\r\n"这种转义字符
			string & par = pars.at(i);
			// 如果首字符是@，表明取消转义字符
			if ( !par.empty() )
				{
				if ( par.at(0) == char_t('@') )
					par.erase(par.begin());
				else if ( par.find_first_of(char_t('\\')) != string::npos )
					sp::trans_escape_char(par);
				}
			push_back(par);
			}
		}
	asc_par_t(void)
		{
		}

	const char_t * par_name(void) const
		{
		if ( !empty() )
			{
			const par_attr & att = front();
			if ( att.no_name() )
				return att.is_null() ? 0 : att.par_value().c_str();
			else
				return att.par_name().c_str();
			}
		return 0;
		}
	const char_t * par_value(void) const
		{
		if ( !empty() )
			return front().par_value().c_str();
		return 0;
		}
	int int_value(void) const
		{
		if ( !empty() )
			return front().int_value();
		return 0;
		}
	unsigned int uint_value(void) const
		{
		if ( !empty() )
			return front().uint_value();
		return 0;
		}
	double double_value(void) const
		{
		if ( !empty() )
			return front().double_value();
		return 0;
		}

	const par_attr & attr_at(int idx) const
		{
		static par_attr empty_attr;
		if ( idx < 0 || attr_count() <= idx )
			return empty_attr;
		return at(idx);
		}

	const char_t * first_value(void) const
		{
		if ( empty() )
			return 0;
		return front().par_value().c_str();
		}
	const string & first_value_string(void) const
		{
		if ( empty() )
			{
			static string str;
			return str;
			}
		return front().par_value();
		}
	const string & second_value_string(void) const
		{
		return attr_at(1).par_value();
		}

	int attr_count(void) const
		{
		return (int)size();
		}
	bool is_null(void) const
		{
		return size() == 1 && front().is_null();
		}

	bool operator == (const asc_par_t & par) const
		{
		if ( par_name() == 0 || par.par_name() == 0 )
			return false;
		return sp::strcasecmp(par_name(), par.par_name()) == 0;
		}
	bool operator == (const string & par) const
		{
		if ( par_name() == 0 )
			return false;
		return sp::strcasecmp(par_name(), par.c_str()) == 0;
		}

	par_attr get_param(const char_t * key)
		{
		iterator pi = std::find(begin(), end(), par_name);
		if ( pi != end() )
			return *pi;
		return par_attr();
		}

	const char_t * get_str_value(const char_t *key, const char_t *default_val = 0) const
		{
		const_iterator pi = std::find(begin(), end(), key);
		if ( pi != end() )
			{
			if ( (*pi).is_null() )
				return 0;
			return (*pi).par_value().c_str();
			}
		return default_val;
		}
	const char_t * get_str_value(const char_t *keys[], int count, const char_t * default_val = 0) const
		{
		const char_t dummy = { '\0' };
		for ( int i = 0; i < count; i++ )
			{
			const char * val = get_str_value(keys[i], dummy);
			if ( val != dummy )
				return val;
			}
		return default_val;
		}

	int get_int_value(const char_t * key, int default_val = 0) const
		{
		const_iterator pi = std::find(begin(), end(), key);
		if ( pi != end() )
			return (*pi).int_value();
		return default_val;
		}
	int get_int_value(const char_t *keys[], int count, int default_val = 0) const
		{
		for ( int i = 0; i < count; i++ )
			{
			string val = get_str_value(keys[i]);
			if ( !val.empty() )
				return sp::strtoi(val.c_str());
			}
		return default_val;
		}

	#ifdef _MSC_VER
	# pragma warning(push)
	# pragma warning(disable : 4311)
	# pragma warning(disable : 4312)
	#endif // VC
	template <class T>
	T get_int_value(const char_t * key, const T & default_val) const
		{
		const_iterator pi = std::find(begin(), end(), key);
		if ( pi != end() )
			return (T)(*pi).int_value();
		return (T)default_val;
		}
	#ifdef _MSC_VER
	# pragma warning(pop)
	#endif // VC
	};


/** 
 * @class	class asc_par_arr
 * @brief	参数列表类
 * 
 *  一个命令的所有参数都存放在这个类中
 *  参数之间以逗号区分，参数的属性对之间以空格区分
 * 
 * @author	jdyu
 * @date	2007-7-3
 * 
 * @see		
 * 
 */
template <typename char_t = char>
class asc_par_arr
	: public std::vector<asc_par_t<char_t> >
	{
public:
	typedef asc_par_t<char_t>			cmd_par;
	typedef std::basic_string<char_t>	string;
	typedef std::vector<string>			str_arr;

	typedef typename std::vector<asc_par_t<char_t> >::iterator	iterator;
	typedef typename std::vector<asc_par_t<char_t> >::const_iterator const_iterator;

	asc_par_arr(const char_t * str = 0)
		{
		if ( str )
			parse(str);
		}
	int parse(const char_t * str)
		{
		if ( str != 0 )
			{
			// 切分之
			str_arr pars;
			const char_t spliter[] = { ',', '\0' };
			sp::split_str(str, pars, spliter, true, false, true, false);
			// 生成列表
			typename str_arr::iterator pi = pars.begin();
			for ( ; pi != pars.end(); pi++ )
				this->push_back(cmd_par(*pi));
			}
		return 0;
		}

	const cmd_par & first_par(void) const
		{
		return par_at(0);
		}
	const cmd_par & second_par(void) const
		{
		return par_at(1);
		}
	const cmd_par & third_par(void) const
		{
		return par_at(2);
		}
	const cmd_par & fourth_par(void) const
		{
		return par_at(3);
		}
	const cmd_par & fifth_par(void) const
		{
		return par_at(4);
		}

	const cmd_par & par_at(int idx) const
		{
		static cmd_par empty_par;
		if ( idx < 0 || par_count() <= idx )
			return empty_par;
		return at(idx);
		}
	int par_count(void) const
		{
		return (int)size();
		}

	const char_t * first_value(void) const
		{
		if ( empty() )
			return 0;
		return front().first_value();
		}
	const string & first_value_string(void) const
		{
		if ( empty() )
			{
			static string str;
			return str;
			}
		return front().first_value_string();
		}
	bool is_null(void) const
		{
		if ( size() == 1 && front().is_null() )
			return true;
		return false;
		}

	bool validate_par(int par_idx) const
		{
		return par_idx >=0 && par_idx < par_count() && !par_at(par_idx).empty();
		}
	bool validate_par(const char_t * par_name) const
		{
		const_iterator pi = std::find(begin(), end(), par_name) != end();
		return pi != end() && !(*pi).empty();
		}
	cmd_par & get_par(const char_t * par)
		{
		static cmd_par empty_par;
		iterator pi = std::find(begin(), end(), par);
		if ( pi != end() )
			return *pi;
		return empty_par;
		}

	// 判断参数是否为NULL
	bool first_par_null(void) const
		{
		return validate_par(0) && par_at(0).is_null();
		}
	bool second_par_null(void) const
		{
		return validate_par(1) && par_at(1).is_null();
		}
	bool third_par_null(void) const
		{
		return validate_par(2) && par_at(2).is_null();
		}
	bool fourth_par_null(void) const
		{
		return validate_par(3) && par_at(3).is_null();
		}
	bool fifth_par_null(void) const
		{
		return validate_par(4) && par_at(4).is_null();
		}
	bool sixth_par_null(void) const
		{
		return validate_par(5) && par_at(5).is_null();
		}

	// 参数有效否
	bool first_par_valid(void) const
		{
		return validate_par(0);
		}
	bool second_par_valid(void) const
		{
		return validate_par(1);
		}
	bool third_par_valid(void) const
		{
		return validate_par(2);
		}
	bool fourth_par_valid(void) const
		{
		return validate_par(3);
		}
	bool fifth_par_valid(void) const
		{
		return validate_par(4);
		}
	bool sixth_par_valid(void) const
		{
		return validate_par(5);
		}

	// 参数属性值的获取
	const char_t * attr_str_value(const char_t *par, const char_t *attr, const char_t *default_val = 0) const
		{
		const_iterator pi = std::find(begin(), end(), par);
		if ( pi != end() )
			return (*pi).get_str_value(attr, default_val);
		return default_val;
		}
	const char_t * first_attr_str_value(const char_t * attr, const char_t *default_val = 0) const
		{
		return first_par().get_str_value(attr, default_val);
		}
	const char_t * second_attr_str_value(const char_t * attr, const char_t *default_val = 0) const
		{
		return second_par().get_str_value(attr, default_val);
		}
	const char_t * third_attr_str_value(const char_t * attr, const char_t *default_val = 0) const
		{
		return third_par().get_str_value(attr, default_val);
		}

	int attr_int_value(const char_t *par, const char_t *attr, int default_val = 0) const
		{
		const_iterator pi = std::find(begin(), end(), par);
		if ( pi != end() )
			return (*pi).get_int_value(attr, default_val);
		return default_val;
		}
	int first_attr_int_value(const char_t *attr, int default_val = 0) const
		{
		return first_par().get_int_value(attr, default_val);
		}
	int second_attr_int_value(const char_t *attr, int default_val = 0) const
		{
		return second_par().get_int_value(attr, default_val);
		}
	int third_attr_int_value(const char_t *attr, int default_val = 0) const
		{
		return third_par().get_int_value(attr, default_val);
		}
	template <class T>
	T first_attr_int_value(const char_t *par, const T & default_val = 0) const
		{
		return first_par().get_int_value(par, default_val);
		}
	template <class T>
	T second_attr_int_value(const char_t *par, const T & default_val = 0) const
		{
		return second_par().get_int_value(par, default_val);
		}
	template <class T>
	T third_attr_int_value(const char_t *par, const T & default_val = 0) const
		{
		return third_par().get_int_value(par, default_val);
		}

	// 参数值获取
	const char_t * par_str_value(int idx, const char_t *par = 0, const char_t *default_val = 0) const
		{
		if ( par )
			{
			const_iterator pi = std::find(begin(), end(), par);
			if ( pi != end() )
				return (*pi).par_value();
			}
		if ( validate_par(idx) )
			return par_at(idx).par_value();

		return default_val;
		}
	const char_t * first_par_str_value(const char_t *par = 0, const char_t *default_val = 0) const
		{
		return par_str_value(0, par, default_val);
		}
	const char_t * second_par_str_value(const char_t *par = 0, const char_t *default_val = 0) const
		{
		return par_str_value(1, par, default_val);
		}
	const char_t * third_par_str_value(const char_t *par = 0, const char_t *default_val = 0) const
		{
		return par_str_value(2, par, default_val);
		}
	const char_t * fourth_par_str_value(const char_t *par = 0, const char_t *default_val = 0) const
		{
		return par_str_value(3, par, default_val);
		}
	const char_t * fifth_par_str_value(const char_t *par = 0, const char_t *default_val = 0) const
		{
		return par_str_value(4, par, default_val);
		}
	const char_t * sixth_par_str_value(const char_t *par = 0, const char_t *default_val = 0) const
		{
		return par_str_value(5, par, default_val);
		}

	int par_int_value(int idx, const char_t *par = 0, int default_val = 0) const
		{
		if ( par )
			{
			const_iterator pi = std::find(begin(), end(), par);
			if ( pi != end() )
				return (*pi).int_value();
			}
		if ( validate_par(idx) )
			return par_at(idx).int_value();

		return default_val;
		}

	#ifdef _MSC_VER
	# pragma warning(push)
	# pragma warning(disable : 4311)
	# pragma warning(disable : 4312)
	#endif // VC
	template <class T>
	T par_int_value(int idx, const char_t *par, const T & default_val) const
		{
		int def = int(default_val);
		int val = par_int_value(idx, par, def);
		return (T)(val);
		}
	#ifdef _MSC_VER
	template <>
	bool par_int_value(int idx, const char_t *par, const bool & default_val) const
		{
		int def = int(default_val);
		int val = par_int_value(idx, par, def);
		return val != 0;
		}
	# pragma warning(pop)
	#endif // VC

	template <class T>
	T first_par_int_value(const char_t *par = 0, const T & default_val = T(0)) const
		{
		return par_int_value<T>(0, par, default_val);
		}
	template <class T>
	T second_par_int_value(const char_t *par = 0, const T & default_val = T(0)) const
		{
		return par_int_value<T>(1, par, default_val);
		}
	template <class T>
	T third_par_int_value(const char_t *par = 0, const T & default_val = T(0)) const
		{
		return par_int_value<T>(2, par, default_val);
		}
	template <class T>
	T fourth_par_int_value(const char_t *par = 0, const T & default_val = T(0)) const
		{
		return par_int_value<T>(3, par, default_val);
		}
	template <class T>
	T fifth_par_int_value(const char_t *par = 0, const T & default_val = T(0)) const
		{
		return par_int_value<T>(4, par, default_val);
		}
	template <class T>
	T sixth_par_int_value(const char_t *par = 0, const T & default_val = T(0)) const
		{
		return par_int_value<T>(5, par, default_val);
		}
	template <class T>
	T seven_par_int_value(const char_t *par = 0, const T & default_val = T(0)) const
	{
		return par_int_value<T>(6, par, default_val);
	}
	};

// 常见类型
typedef asc_par_t<char>			cmd_par;
typedef asc_par_t<wchar_t>		wcmd_par;
typedef asc_par_arr<char>		par_arr;
typedef asc_par_arr<wchar_t>	wpar_arr;

/** 
 * @class	asc_parser_i
 * 
 * @brief	解析器的接口
 * 
 *  用户可以派生，达到其它的解析功能
 * 
 * @author	jdyu
 * @date	2007-6-13
 * 
 */
template <class SINK, class char_t>
struct asc_parser_it
	{
	virtual int parse_file  (const char_t * asc_file) = 0;
	virtual int parse_string(const char_t * asc_string) = 0;
	virtual int parse_stop	(void) = 0;

	virtual int add_parse_sink(SINK * sink, void * rev = 0) = 0;
	virtual int remove_parse_sink(SINK * sink) = 0;
	virtual int clear_sinks(void) = 0;

	virtual const char_t * script_file(void) const = 0;
	virtual const char_t * script_string(void) const = 0;

	virtual unsigned int line_no(void) const = 0;
	virtual const char_t * line_str(void) const = 0;
	virtual int loop_cursor(void) const = 0;
	};

/** 
 * @class	asc_sink_i
 * 
 * @brief	命令响应器接口
 * 
 *  脚本解析器的响应接口，可以跨dll使用
 * 
 * @author	jdyu
 * @date	2007-6-13
 * 
 */
template <class char_t>
struct asc_sink_it
	{
	typedef asc_parser_it<asc_sink_it, char_t> PARSER;

	// 派生类需要实现的响应函数
	virtual const char_t * name (void) const = 0;

	virtual int on_begin		(void) = 0;
	virtual int on_end			(void) = 0;
	virtual int on_parse_begin	(PARSER * parser) = 0;
	virtual int on_parse_end	(void) = 0;
	virtual int on_batch_begin	(void) = 0;
	virtual int on_batch_end	(void) = 0;

	virtual int on_cmd_begin	(const char_t * cmd_name, const char_t * cmd_text)	= 0;
	virtual int on_cmd_end		(const char_t * cmd_name, const char_t * cmd_text, size_t text_len)	= 0;
	virtual int on_cmd_line		(const char_t * cmd_name, const char_t * line_str, int line_no)	= 0;
	virtual int on_variable     (const char_t * cmd_name, const char_t * var_name, const char_t *& result, size_t & len) = 0;
	};

// 宽窄字符版的接收器
typedef asc_sink_it<char>					asc_sink_i;
typedef asc_sink_it<wchar_t>				wasc_sink_i;
// 宽窄字符版的解析器
typedef asc_parser_it<asc_sink_i,  char>	asc_parser_i;
typedef asc_parser_it<wasc_sink_i, wchar_t>	wasc_parser_i;

// 接收器的缺省实现
template <class SINK = asc_sink_i, class char_t = char, class PARSER = asc_parser_i>
class asc_sink_t
	: public SINK
	{
public:
	// 派生类需要实现的响应函数
	virtual int on_begin(void)
		{
		dslog_trace("on_begin.");
		return 0;
		}
	virtual int on_end(void)
		{
		dslog_trace("on_end.");
		return 0;
		}
	virtual int on_parse_begin(PARSER * parser)
		{
		if ( parser )
			scp_parser_  = parser;
		dslog_trace("on_parse_begin.");
		return 0;
		}
	virtual int on_parse_end(void)
		{
		dslog_trace("on_parse_end.");
		return 0;
		}
	virtual int on_batch_begin(void)
		{
		dslog_trace("on_batch_begin.");
		return 0;
		}
	virtual int on_batch_end(void)
		{
		dslog_trace("on_batch_end.");
		return 0;
		}

	virtual int on_cmd_begin(const char_t * cmd_name, const char_t * cmd_text)
		{
		dslog_trace("on_cmd_begin.");
		return 0;
		}
	virtual int on_cmd_end(const char_t * cmd_name, const char_t * cmd_text, size_t text_len)
		{
		dslog_trace("on_cmd_end cmd_name %s.", cmd_name);
		return 0;
		}
	virtual int on_cmd_line(const char_t * cmd_name, const char_t * line_str, int line_no)
		{
		dslog_trace("on_cmd_line cmd_name %s, line_no.", cmd_name, line_no);
		return 0;
		}
	virtual int on_variable(const char_t * cmd_name, const char_t * var_name, const char_t *& result, size_t & len)
		{
		result = var_name;
		len    = sp::strlen(var_name);
		dslog_trace("on_variable, cmd_name %s, var_name %s.", cmd_name, var_name);
		return 0;
		}

	PARSER * parser(void) const
		{
		return scp_parser_;
		}
	const char_t * scp_file(void) const
		{
		return parser()->script_file();
		}
	const char_t * scp_string(void) const
		{
		return parser()->script_string();
		}
	virtual const char_t * name (void) const
		{
		return name_.c_str();
		}
	void name(const char_t * nm)
		{
		if ( nm ) name_ = nm;
		}

	asc_sink_t(const char_t * name = 0, PARSER * scp_parser = 0)
		: scp_parser_	(scp_parser)
		{
		if ( name )
			name_ = name;
		}
	virtual ~asc_sink_t(void)
		{
		}

protected:
	// type defines
	typedef std::basic_string<char_t>	string;
	typedef std::vector<string>			str_arr;
	typedef asc_par_t<char_t>			cmd_par;
	typedef asc_par_arr<char_t>			par_arr;

	// 内部函数
	int get_cmd_detail(const char_t * cmd_str, string & ret_str, par_arr & par_list)
		{
		ret_str.clear();

		// 应该的返回值
		const char_t should_return[] = { 'r', 'e', 't', 'u', 'r', 'n', '\0' };

		// 切分之
		str_arr pars;
		const char_t spliter[] = { ';', ',', '\0' };
		sp::split_str(cmd_str, pars, spliter, true, false, true, false);
		if ( pars.empty() )
			return 0;

		// 找到应该的返回值
		typename str_arr::iterator pi = pars.begin();
		for ( ; pi != pars.end(); pi++ )
			{
			cmd_par par(*pi);
			if ( par.par_name() && sp::strcasecmp(par.par_name(), should_return) == 0 )
				ret_str = par.par_value();
			else
				par_list.push_back(par);
			}

		return 0;
		}
	// 返回脚本指定的运行册数
	unsigned int run_rounds(const par_arr & pars) const
		{
		// 是否仅需要执行一次
		const char_t run_rounds[] = { 'r', 'o', 'u', 'n', 'd', 's', '\0' };
		if ( !pars.empty() )
			{
			const cmd_par & last_par = pars.back();
			if ( last_par.par_name() && sp::strcasecmp(last_par.par_name(), run_rounds) == 0 )
				return sp::strtoui(last_par.par_value());
			}

		return 0;
		}

private:
	// 解析器接口
	PARSER *	scp_parser_;
	// 名字
	string		name_;
	};

// ============================================================================
// 自动化脚本的实现
// 通用的脚本解析器
// ============================================================================
template <class BASE = asc_parser_i, class SINK = asc_sink_i, class char_t = char>
class asc_parser_t
	: public BASE
	{
protected:
	// type defines
	typedef std::basic_string<char_t> string;
	typedef std::vector<string>	str_arr;
	typedef unsigned int uint_t;

	// const values
	enum char_val
		{
		char_zero		= '\0',
		char_tab		= '\t',
		char_cr			= '\r',
		char_lf			= '\n',
		char_sharp		= '#',
		char_semi		= ';',
		char_brace_l	= '{',
		char_brace_r	= '}',
		char_braket_l	= '(',
		char_braket_r	= ')',
		char_dollar		= '$',
		char_space		= ' ',
		char_comma		= ',',
		char_colon		= ':',
		char_backslash	= '\\',
		char_esc		= '\\',
		char_slash		= '/',
		char_star		= '*',
		};

	// 一行脚本的类型
	enum line_cate
		{
		line_null,
		line_label,
		line_cmd,
		line_incmd,
		line_loop_begin,
		line_loop_end,
		line_goto,
		};

	// 支持的关键字
	enum asc_keywords
		{
		key_for,
		key_forend,
		key_goto,
		};

	// 一行最大字符数
	enum
		{
		line_max_len = 32 * 1024,
		};

	// 派生类需要实现的响应函数
	virtual int on_cmd_begin	(const char_t * cmd_name, const char_t * cmd_text)
		{
		typename sink_list::iterator pi = parse_sinks_.begin();
		for ( ; pi != parse_sinks_.end(); pi++ )
			(*pi)->on_cmd_begin(cmd_name, cmd_text);
		return 0;
		}
	virtual int on_cmd_end		(const char_t * cmd_name, const char_t * cmd_text, size_t text_len)
		{
		typename sink_list::iterator pi = parse_sinks_.begin();
		for ( ; pi != parse_sinks_.end(); pi++ )
			(*pi)->on_cmd_end(cmd_name, cmd_text, text_len);
		return 0;
		}
	virtual int on_cmd_line		(const char_t * cmd_name, const char_t * line_str, int line_no)
		{
		typename sink_list::iterator pi = parse_sinks_.begin();
		for ( ; pi != parse_sinks_.end(); pi++ )
			(*pi)->on_cmd_line(cmd_name, line_str, line_no);
		return 0;
		}
	virtual int on_variable     (const char_t * cmd_name, const char_t * var_name, string & result)
		{
		typename sink_list::iterator pi = parse_sinks_.begin();
		for ( ; pi != parse_sinks_.end(); pi++ )
			{
			const char_t * res = 0;
			size_t len = 0;
			int ret = (*pi)->on_variable(cmd_name, var_name, res, len);
			if ( res )
				{
				result.assign(res, len);
				return ret;
				}
			}
		return 0;
		}
	virtual int on_parse_begin(void)
		{
		typename sink_list::iterator pi = parse_sinks_.begin();
		for ( ; pi != parse_sinks_.end(); pi++ )
			(*pi)->on_parse_begin(this);
		return 0;
		}
	virtual int on_parse_end(void)
		{
		typename sink_list::iterator pi = parse_sinks_.begin();
		for ( ; pi != parse_sinks_.end(); pi++ )
			(*pi)->on_parse_end();
		return 0;
		}

public:
	// 设置解析接收器
	virtual int add_parse_sink(SINK * sink, void * rev = 0)
		{
		SP_ASSERT(sink);
		if ( std::find(parse_sinks_.begin(), parse_sinks_.end(), sink) != parse_sinks_.end() )
			{
			dslog_error_assert(false,
				("add_parse_sink sink 0x%x already exists.", sink));
			return 0;
			}
		parse_sinks_.push_back(sink);
		return 0;
		}
	virtual int remove_parse_sink(SINK * sink)
		{
		SP_ASSERT(sink);
		typename sink_list::iterator pi = std::find(parse_sinks_.begin(), parse_sinks_.end(), sink);
		if ( pi == parse_sinks_.end() )
			{
			dslog_error_assert(false,
				("remove_parse_sink sink 0x%x not exists.", sink));
			return -1;
			}
		parse_sinks_.erase(pi);
		return 0;
		}
	virtual int clear_sinks(void)
		{
		parse_sinks_.clear();
		return 0;
		}

	// 属性获得和设置
	virtual const char_t * script_file(void) const
		{
		return scp_file_.c_str();
		}
	virtual const char_t * script_string(void) const
		{
		return scp_text_.c_str();
		}

	virtual unsigned int line_no(void) const
		{
		return line_no_;
		}
	virtual const char_t * line_str(void) const
		{
		return line_str_.c_str();
		}
	virtual int loop_cursor(void) const
		{
		return loop_cur_;
		}

	/** 
	 * @brief 	parse_string
	 * 
	 * 	解析字符串
	 * 
	 * @author	jdyu
	 * @date	2007-5-30
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @param	const char * asc_string	- [in] 
	 * @see		
	 */
	virtual int parse_string(const char_t * asc_string)
		{
		int  lnum = 0;
		char_t line[line_max_len];
		SP_ASSERT(asc_string);

		// 检查是否已经解析过
		if ( !all_lines_.empty() )
			return parse_batch();

		parse_stop_ = false;
		on_parse_begin();

		// get all lines
		for ( int i = 0, j = 0; !parse_stop_; i++ )
			{
			if ( asc_string[i] == char_cr ||
				 asc_string[i] == char_lf ||
				 asc_string[i] == char_zero )
				{
				line[j] = 0;
				if ( !in_cmd_ )
					sp::trim_str(line);

				// parse cur line
				parse_line(line, ++lnum);
				if ( line_cat_ == line_loop_end )
					batch_parse_last_for();
				j = 0;
				}
			else if ( j < SP_COUNTOF(line) )
				line[j++] = asc_string[i];

			if ( asc_string[i] == 0 )
				break;
			}

		on_parse_end();
		return 0;
		}

	/** 
	 * @brief 	parse_file
	 * 
	 * 	开始解析指定的文件
	 * 
	 * @author	jdyu
	 * @date	2007-5-30
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @param	const char * asc_file	- [in] 
	 * @see		
	 */
	virtual int parse_file(const char_t * asc_file)
		{
		int ret = 0;
		parse_stop_ = false;

		// 检查是否已经解析过
		if ( !all_lines_.empty() )
			return parse_batch();

		// check file
		if ( asc_file == 0 )
			{
			dslog_error("asc_file is null!");
			return -1;
			}

		#ifdef WIN32
		std::basic_ifstream<char_t, std::char_traits<char_t> > fs(asc_file);
		#else // Lnx
		std::basic_ifstream<char_t, std::char_traits<char_t> > fs(sp::tochar(asc_file).c_str());
		#endif // OS
		if ( fs.fail() )
			{
			dslog_error("Cannot open asc_file %s, errno %d!", asc_file, errno);
			return -1;
			}

		#ifdef WIN32
		fs.imbue(std::locale(""));
		#endif // WIN32
		scp_file_ = asc_file;
		on_parse_begin();

		// read all file
		int    lnum = 0;
		char_t line[line_max_len];
		string line_cont;
		while ( !parse_stop_ && !fs.eof() )
			{
			line[0] = 0;
			fs.getline(line, SP_COUNTOF(line));

			int len = 0;
			if ( !in_cmd_ )
				len = sp::trim_str(line);

			// 有否续行
			if ( len > 0 && line[len-1] == char_backslash )
				{
				line[len-1] = 0;
				line_cont += line;
				lnum ++;
				continue ;
				}

			// parse this line
			parse_line(line_cont.empty() ? line : line_cont.c_str(), lnum++);
			if ( line_cat_ == line_loop_end )
				batch_parse_last_for();

			line_cont.clear();
			}

		on_parse_end();
		return ret;
		}
	virtual int parse_stop(void)
		{
		parse_stop_ = true;
		return 0;
		}

	// 离线解析，用于第二次解析
	int parse_batch(void)
		{
		parse_stop_ = false;
		if ( !all_lines_.empty() )
			{
			on_parse_begin();
			batch_parse_from(all_lines_.front().line_num());
			on_parse_end();
			}
		return 0;
		}
	string & cmd_content(void)
		{
		return cmd_content_;
		}

	// 构造函数
	asc_parser_t(void)
		: in_cmd_		(false)
		, in_comment_	(false)
		, parse_stop_	(false)
		, line_cat_		(line_null)
		, loop_cur_		(0)
		{
		}
	virtual ~asc_parser_t(void)
		{
		}

protected:
	/** 
	 * @brief 	parse_line
	 * 
	 * 	解析一行脚本
	 * 
	 * @author	jdyu
	 * @date	2007-5-30
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char * line	- [in] 一行的内容和行号
	 * @see		
	 */
	virtual int parse_line(const char_t * line, int line_number, bool add_line = true)
		{
		line_str_ = line;
		line_no_  = line_number;
		line_cat_ = line_null;

		// 空行
		const char_t crlf[] = { char_cr, char_lf, char_zero };
		if ( line[0] == 0 )
			{
			dslog_trace("Script_Parser::parse_line empty line, line number %d.", line_number);
			if ( in_cmd_ )
				{
				on_cmd_line(cur_cmd_.c_str(), line, line_number);
				if ( add_line )
					push_line(line_str_, line_incmd, line_number);
				cmd_content_ += crlf;
				}
			return 0;
			}

		// 块状注释 (修复脚本中存在 “/* */”注释的情况 fixed by cnliu)
		if ( line[0] == char_slash && line[1] == char_star )
			{
			in_comment_ = true;
			dslog_trace("Script_Parser::parse_line comment line begin, line_no %d.", line_number);

			size_t line_len = sp::strlen(line);
			if ( line[line_len-2] == char_star && line[line_len-1] == char_slash )
				in_comment_ = false;
			dslog_trace("Script_Parser::parse_line comment line end, line_no %d.", line_number);
			return 0;
			}
		if ( in_comment_ )
			{
			if ( line[0] == char_star && line[1] == char_slash )
				{
				in_comment_ = false;
				dslog_trace("Script_Parser::parse_line comment line end, line_no %d.", line_number);
				return 0;
				}
			dslog_trace("Script_Parser::parse_line comment line %s, line_no %d.", line, line_number);
			return 0;
			}

		// 转义字符
		int start = 0;
		if ( line[0] != char_esc )
			{
			// 注释行
			if ( line[0] == char_sharp  /* '#' */
				|| line[0] == char_semi /* ';' */ )
				{
				dslog_trace("Script_Parser::parse_line comment line %s, line_no %d.", line, line_number);
				return 0;
				}
			}
		else
			line++;

		// logging
		dslog_trace("Script_Parser::parse_line %d, content %s", line_number, line);

		// 命令字
		if ( in_cmd_ == false )
			{
			// begin of command
			if ( line[0] == char_brace_l /* "{" */
				&& line[1] == char_zero )
				{
				in_cmd_ = true;
				if ( add_line )
					push_line(line_str_, line_incmd, line_number);
				}
			else
				{
				std::vector<string> cmds;
				const char_t spliter[] = { char_space, char_tab, char_zero };

				// 有没有变量
				string line_res;
				const char_t * beg = line;
				if ( sp::strchr(line, char_dollar) != 0 )
					{
					if ( process_vars(line, line_res) != 0 )
						{
						dslog_verbose("Script_Parser::line [%d] %s has bad variable.", line_number, line);
						}
					beg = line_res.c_str();
					}
				// split to cmd and pars
				sp::split_str(beg, cmds, spliter, true, false, true);

				const string & cn = cmds.front();
				const char_t * pc = beg + cn.length();
				while (uint_t(*pc) <= 0x20 && *pc != 0) pc++;

				// command begin line
				on_cmd_begin(cn.c_str(), pc);
				if ( add_line )
					push_line(line_str_, line_number, cmds);

				cmd_line_ = beg;
				cur_cmd_  = cn;
				cmd_content_.clear();
				}
			}
		else
			{
			// end of cmd
			if ( line[0] == char_brace_r /* "}" */
				&& ( line[1] == char_zero || line[1] == '\r' || line[1] == '\n' ) )
				{
				on_cmd_end(cur_cmd_.c_str(), cmd_content_.c_str(), cmd_content_.size());
				if ( add_line )
					push_line(line_str_, line_incmd, line_number);
				cur_cmd_.clear();
				in_cmd_ = false;
				}
			else
				{
				// 行内是否有变量
				string line_res;
				const char_t * beg = line;
				if ( sp::strchr(line, char_dollar) != 0 )
					{
					if ( process_vars(line, line_res) != 0 )
						{
						dslog_verbose("Script_Parser::line [%d] %s has bad variable.", line_number, line);
						}
					beg = line_res.c_str();
					}

				// 每一行有效数据回调
				on_cmd_line(cur_cmd_.c_str(), beg, line_number);
				if ( add_line )
					push_line(line_str_, line_incmd, line_number);

				cmd_content_ += beg == line ? line : line_res;
				cmd_content_ += crlf;
				}
			}

		return 0;
		}

	// 处理行内有变量的情况
	int process_vars(const char_t * line, string & line_res)
		{
		int ret = 0;
		string line_str = line;

		typename string::size_type pd = line_str.find_first_of(char_dollar);
		if ( pd != string::npos )
			{
			// 行内的变量被处理后的文本
			line_res.assign(line, pd);

			while ( pd != string::npos )
				{
				size_t pc = line_str.find_first_of(char_braket_r, pd);	/* ')' */
				if ( pc != string::npos )
					{
					string var;
					var.assign(line_str.c_str() + pd + 1, pc - pd);
					const char_t to_trim[] = { char_braket_r, char_braket_l, char_space, char_zero };
					sp::trim_str(var);

					string res;
					on_variable(cur_cmd_.c_str(), var.c_str(), res);

					line_res += res;

					// find next var
					pd = line_str.find_first_of(char_dollar, ++pc);		/* '$' */
					if ( pd != string::npos )
						line_res.append(line_str, pc, pd - pc);
					else
						line_res.append(line_str.c_str() + pc);
					}
				else
					{
					ret = -1;
					line_res = line;
					break;
					}
				}
			return 0;
			}
		return ret;
		}

	// 增加记录一行
	int push_line(const string & line, line_cate cat, uint_t line_no)
		{
		if ( cat == line_loop_begin )
			{
			lines_rit pi = all_lines_.rbegin();
			for ( ; pi != all_lines_.rend(); pi++ )
				{
				if ( pi->line_cat() == line_loop_begin )
					return -1;
				if ( pi->line_cat() == line_loop_end )
					break;
				}
			}
		else if ( cat == line_loop_end )
			{
			lines_rit pi = all_lines_.rbegin();
			for ( ; pi != all_lines_.rend(); pi++ )
				{
				if ( pi->line_cat() == line_loop_end )
					return -1;
				if ( pi->line_cat() == line_loop_begin )
					{
					// 只有发现begin才加入
					all_lines_.push_back(asc_line(line, cat, line_no));
					line_cat_ = line_loop_end;
					return 0;
					}
				}
			}

		line_cat_ = cat;
		all_lines_.push_back(asc_line(line, cat, line_no));
		return 0;
		}
	int push_line(const string & line, uint_t line_no, str_arr & cmds)
		{
		SP_ASSERT(!cmds.empty());
		static const char_t supp_keys[][32] =
			{
				{ 'f', 'o', 'r', '\0' },
				{ 'e', 'n', 'd', '\0' },
				{ 'g', 'o', 't', 'o', '\0' },
			};

		string & cmd_name = cmds.front();
		switch ( sp::search_strarr(cmd_name.c_str(), supp_keys, false) )
			{
			case key_for:
				push_line(line, line_loop_begin, line_no);
				if ( cmds.size() > 1 )
				{
					str_arr pars;
					const char_t splt[] = { ',', ';', '\0' };
					sp::split_str(line.c_str() + cmds[0].size(), pars, splt);

					asc_line & last_line = all_lines_.back();
					if ( pars.size() > 0 )
						last_line.loop_begin(sp::strtoi(pars[0].c_str()));
					if ( pars.size() > 1 )
						last_line.loop_end  (sp::strtoi(pars[1].c_str()));
					if ( pars.size() > 2 )
						last_line.loop_step (sp::strtoi(pars[2].c_str()));
					// loop cursor to begin
					loop_cur_ = last_line.loop_begin();
				}
				break;
			case key_forend:
				push_line(line, line_loop_end, line_no);
				break;
			case key_goto:
				push_line(line, line_goto, line_no);
				break;
			default:
				push_line(line, line_cmd, line_no);
				break;
			}
		return 0;
		}

	// 从指定的行号开始执行
	int batch_parse_from(uint_t line_no)
		{
		bool in_for = false;
		lines_it pi = std::find(all_lines_.begin(), all_lines_.end(), line_no);
		for ( ; pi != all_lines_.end(); pi++ )
			{
			// parse per line
			if ( !parse_stop_ && !in_for )
				parse_line(pi->c_str(), pi->line_num(), false);

			// for or end for
			if ( pi->line_cat() == line_loop_end )
				in_for = false;
			else if ( pi->line_cat() == line_loop_begin )
				{
				in_for = true;
				for ( int i = pi->loop_begin(); i < pi->loop_end(); i+= pi->loop_step() )
					{
					loop_cur_ = i;
					batch_parse_for(pi->line_num());
					if ( parse_stop_ )
						break;
					}
				}
			}
		return 0;
		}

	// 执行指定行号的for循环
	int batch_parse_for(uint_t line_no)
		{
		lines_it pi = std::find(all_lines_.begin(), all_lines_.end(), line_no);
		for ( ; pi != all_lines_.end(); pi++ )
			{
			if ( parse_stop_ || pi->line_cat() == line_loop_end )
				break;
			if ( pi->line_cat() != line_loop_begin )
				parse_line(pi->c_str(), pi->line_num(), false);
			}
		return 0;
		}

	// 从最后一个for开始解析
	int batch_parse_last_for(void)
		{
		for ( lines_rit pi = all_lines_.rbegin(); pi != all_lines_.rend(); pi++ )
			{
			if ( pi->line_cat() == line_loop_begin )
				{
				for ( int i = pi->loop_begin() + pi->loop_step(); i < pi->loop_end(); i+= pi->loop_step() )
					{
					loop_cur_ = i;
					batch_parse_for(pi->line_num());
					if ( parse_stop_ )
						break;
					}
				break;
				}
			loop_cur_ = 0;
			}
		return 0;
		}

	// 一行脚本的信息
	class asc_line
		{
	public:
		asc_line(const string & line, line_cate cat, uint_t lineno)
			: line_str_	(line)
			, line_num_	(lineno)
			, line_cat_	(cat)
			, loop_beg_	(0)
			, loop_end_	(0)
			, loop_step_(1)
			{
			}
		string & line_str(void)
			{
			return line_str;
			}
		const string & line_str(void) const
			{
			return line_str_;
			}
		const char_t * c_str(void) const
			{
			return line_str_.c_str();
			}

		line_cate line_cat(void) const
			{
			return line_cat_;
			}
		uint_t line_num(void) const
			{
			return line_num_;
			}

		int loop_begin(void) const
			{
			return loop_beg_;
			}
		int loop_end(void) const
			{
			return loop_end_;
			}
		int loop_step(void) const
			{
			return loop_step_;
			}
		void loop_begin(int beg)
			{
			loop_beg_ = beg;
			}
		void loop_end(int end)
			{
			loop_end_ = end;
			}
		void loop_step(int step)
			{
			loop_step_ = step;
			}

		bool operator == (uint_t line_no) const
			{
			return line_num_ == line_no;
			}

	private:
		// 文本内容
		string		line_str_;
		uint_t		line_num_;
		// 行类型
		line_cate	line_cat_;
		// 循环计数
		int			loop_beg_;
		int			loop_end_;
		int			loop_step_;
		};
	// 所有的行集合
	typedef std::vector<asc_line> asc_lines;
	typedef typename asc_lines::iterator lines_it;
	typedef typename asc_lines::reverse_iterator lines_rit;

private:
	// 脚本文本
	string			scp_text_;
	// 脚本文件
	string			scp_file_;

	// 当前的命令字
	string			cur_cmd_;
	// 是否在命令字内部
	bool			in_cmd_;
	bool			in_comment_;
	bool			parse_stop_;

	// 当前命令内容
	string			cmd_line_;
	string			cmd_content_;

	// 当前行信息
	uint_t			line_no_;
	string			line_str_;

	// 上一行的类型
	line_cate		line_cat_;
	int				loop_cur_;

	// 接收器指针列表
	typedef std::list<SINK*> sink_list;
	sink_list		parse_sinks_;

	// 存放解析过的所有行内容
	asc_lines		all_lines_;
	};

// 宽窄字符版本的解析器
typedef asc_parser_t<asc_parser_i,  asc_sink_i,  char>		asc_parser;
typedef asc_parser_t<wasc_parser_i, wasc_sink_i, wchar_t>	wasc_parser;

// ============================================================================
// defines to process command
// ============================================================================
#define asc_cmd_entry_begin(cls)						\
	protected:											\
	virtual const sc::asc_cmd_t<cls>::cmd_info* get_cmd_info(void) const	\
		{												\
		static const cmd_info cmd_map[] = {

#define asc_cmd_entry_begin_w(cls)						\
	protected:											\
	virtual const sc::wasc_cmd_t<cls>::cmd_info* get_cmd_info(void) const	\
		{												\
		static const cmd_info cmd_map[] = {

#define asc_cmd_entry_pars(cls, cmd_name)				\
			{ #cmd_name, & cls::on_##cmd_name, 0, 0, true },
#define asc_cmd_entry_int(cls, cmd_name)				\
			{ #cmd_name, 0, 0, & ##cls::on_##cmd_name, true },

#define asc_cmd_entry_pars_w(cls, cmd_name)				\
			{ L## #cmd_name, & cls::on_##cmd_name, 0, 0, true },
#define asc_cmd_entry_int_w(cls, cmd_name)				\
			{ L## #cmd_name, 0, 0, & ##cls::on_##cmd_name, true },

#define asc_cmd_end_entry(cls, cmd_name)				\
			{ #cmd_name,  0, & cls::on_##cmd_name##_end, 0, true },
#define asc_cmd_end_entry_w(cls, cmd_name)				\
			{ L## #cmd_name, 0, & cls::on_##cmd_name##_end, 0, true },

#define asc_cmd_entry_all(cls, cmd_name)				\
			{  #cmd_name, & cls::on_##cmd_name, & cls::on_##cmd_name##_end, 0, true },
#define asc_cmd_entry_all_w(cls, cmd_name)				\
			{ L## #cmd_name, & cls::on_##cmd_name, & cls::on_##cmd_name##_end, 0, true },

#define asc_cmd_entry_end()								\
			{ 0, 0,	0, 0, false }						\
		};												\
		return cmd_map;	}

#define asc_cmd_proc_decl(cmd_name)						\
	virtual int on_##cmd_name(const sc::par_arr & pars)  { return 0; }
#define asc_cmd_proc_decl_end(cmd_name)					\
	virtual int on_##cmd_name##_end(const sc::par_arr & pars, const std::string & content)  { return 0; }
#define asc_cmd_proc_decl_all(cmd_name)					\
	asc_cmd_proc_decl(cmd_name)							\
	asc_cmd_proc_decl_end(cmd_name)

#define asc_cmd_proc_decl_w(cmd_name)					\
	virtual int on_##cmd_name(const sc::wpar_arr & pars) { return 0; }
#define asc_cmd_proc_decl_end_w(cmd_name)				\
	virtual int on_##cmd_name##_end(const sc::wpar_arr & pars, const std::wstring & content) { return 0; }
#define asc_cmd_proc_decl_all_w(cmd_name)				\
	asc_cmd_proc_decl_w(cmd_name)						\
	asc_cmd_proc_decl_end_w(cmd_name)

#define asc_cmd_proc_decl_i(cmd_name)					\
	virtual int on_##cmd_name(int par) { return 0; }


// ============================================================================
// 命令字分发类
// 实现者请从这个类进行派生，可以大大简化工作量
// ============================================================================
template <class T, class SINK = sc::asc_sink_i, class char_t = char, class PARSER = sc::asc_parser_i>
class asc_cmd_t
	: public sc::asc_sink_t<SINK, char_t, PARSER>
	{
public:
	// type defines
	typedef sc::asc_sink_t<SINK, char_t, PARSER> parent;
	typedef std::basic_string<char_t>	string;
	typedef std::vector<string>			str_arr;
	typedef std::map<string, string>	str_map;
	typedef asc_par_t<char_t>			cmd_par;
	typedef asc_par_arr<char_t>			par_arr;

	// 派生类需要实现的响应函数
	virtual int on_parse_begin(PARSER * parser)
		{
		int ret = parent::on_parse_begin(parser);
		if ( cmd_info_.empty() )
			get_cmd_info(cmd_info_);
		
		bat_cmds_.clear();
		if ( !bat_mode_ )
			return on_begin();
		return ret;
		}
	virtual int on_parse_end(void)
		{
		if ( !bat_mode_ )
			on_end();
		return parent::on_parse_end();
		}
	virtual int on_batch_begin(void)
		{
		parent::on_batch_begin();
		return on_begin();
		}
	virtual int on_batch_end(void)
		{
		on_end();
		return parent::on_end();
		}

	// 单行命令字
	virtual int on_cmd_begin(const char_t * cmd_name, const char_t * cmd_text)
		{
		string  rets;
		par_arr pars;
		string  cmds = cmd_name;

		bool is_label = false;
		if ( !cmds.empty() && cmds.at(cmds.length()-1) == char_t(':') )
			{
			is_label = true;
			sp::trim_str(cmds, char_t(':'));
			if ( sp::strcasecmp(cmds.c_str(), goto_cmd_.c_str()) == 0 )
				{
				goto_cmd_.clear();
				return 0;
				}
			}

		// 是否存在goto语句
		if ( !goto_cmd_.empty() && goto_cmd_ != cmd_name )
			{
			dslog_trace("asc_sink_t::on_cmd_begin skip cmd %s to goto %s.",
				sp::tochar(cmd_name).c_str(), sp::tochar(goto_cmd_).c_str());
			return 0;
			}

		// 获得命令字详细信息
		int ret = get_cmd_detail(cmd_text, rets, pars);
		if ( ret != 0 )
			{
			dslog_error("asc_sink_t::on_cmd_begin get_cmd_detail failed %d.", ret);
			}

		// dispatch command
		bool stat_res = false;
		for ( int i = 0; i < (int)cmd_info_.size(); i++ )
			{
			const cmd_info & ci  = *cmd_info_[i];
			if ( ci.cmd_name_ == 0 )
				break ;

			if ( sp::strcasecmp(cmd_name, ci.cmd_name_) == 0 )
				{
				// 如果是批处理模式，记录命令字
				if ( bat_mode_ )
					{
					bat_cmds_.push_back(bat_cmd(&ci, pars, rets));
					last_pars_ = pars;
					return 0;
					}

				stat_res = ci.stat_result_;
				if ( ci.cmd_proc_int_ )
					{
					int par = pars.empty() ? 0 : sp::strtoi(pars.first_value());
					ret = (dynamic_cast<T*>(this)->*ci.cmd_proc_int_)(par);
					break ;
					}
				else if ( ci.cmd_proc_beg_ )
					{
					ret = (dynamic_cast<T*>(this)->*ci.cmd_proc_beg_)(pars);
					break ;
					}
				}
			}

		// Verify return value
		bool pass = verify_return(cmd_name, rets, ret);
		if ( stat_res )
			cmd_stat_.add_result(cmd_name, ret == 0, pass);
		if ( !pass )
			{
			dslog_error("on_cmd_begin | line = %d, cmd %s return value %d, not pass.",
				parser()->line_no() + 1, sp::tochar(cmd_name).c_str(), ret);
			}

		last_pars_ = pars;
		last_rets_ = rets;
		last_ret_  = ret;
		return 0;
		}

	virtual int on_cmd_end(const char_t * cmd_name, const char_t * cmd_text, size_t text_len)
		{
		int ret = 0;
		// 是否存在goto语句
		if ( !goto_cmd_.empty() && goto_cmd_ != cmd_name )
			{
			dslog_trace("asc_sink_t::on_cmd_end skip cmd %s to goto %s.",
				sp::tochar(cmd_name).c_str(), sp::tochar(goto_cmd_).c_str());
			return 0;
			}

		// dispatch command
		bool stat_res = false;
		for ( int i = 0; i < (int)cmd_info_.size(); i++ )
			{
			const cmd_info & ci  = *cmd_info_[i];
			if ( ci.cmd_name_ == 0 )
				break;

			stat_res = ci.stat_result_;
			if ( sp::strcasecmp(cmd_name, ci.cmd_name_) == 0 )
				{
				// 如果是批处理模式，记录命令字
				if ( bat_mode_ )
					{
					bat_cmds_.push_back(bat_cmd(&ci, last_pars_, last_rets_, cmd_text));
					return 0;
					}

				if ( ci.cmd_proc_end_ )
					{
					ret = (dynamic_cast<T*>(this)->*ci.cmd_proc_end_)(last_pars_, string(cmd_text, text_len));
					break ;
					}
				}
			}

		bool pass = verify_return(cmd_name, last_rets_, ret);
		if ( !pass )
			{
			dslog_error("on_cmd_end | line = %d | cmd %s return value %d not pass.", parser()->line_no(), sp::tochar(cmd_name).c_str(), ret);
			}
		if ( stat_res )
			cmd_stat_.add_result(cmd_name, ret == 0, pass);
		last_ret_ = ret;

		return 0;
		}

	string & log_file(void)
		{
		return log_file_;
		}
	string & audio_file(void)
		{
		return audio_file_;
		}
	const string & audio_file(void) const
		{
		return audio_file_;
		}

	bool bat_mode(void) const
		{
		return bat_mode_;
		}
	void bat_mode(bool bm)
		{
		bat_mode_ = bm;
		}

	int batch_exec(void)
		{
		return batch_exec(bat_cmds_);
		}
	void batch_stop(void)
		{
		bat_stop_ = true;
		if ( parser() )
			parser()->parse_stop();
		}
	bool batch_stopping(void) const
		{
		return bat_stop_;
		}

	asc_cmd_t(bool bat_mod = false)
		: last_ret_ (0)
		, bat_mode_	(bat_mod)
		, bat_stop_	(false)
		{
		}
	virtual ~asc_cmd_t(void)
		{
		}

protected:
	// 定义函数指针类型 
	typedef int (T::*cmd_routine_beg)(const par_arr & par_list);
	typedef int (T::*cmd_routine_end)(const par_arr & par_list, const string & par_str);
	typedef int (T::*cmd_routine_int)(int par);

	// 一个命令字和他的处理信息
	struct cmd_info
	{
		const char_t *		cmd_name_;
		cmd_routine_beg		cmd_proc_beg_;
		cmd_routine_end		cmd_proc_end_;
		cmd_routine_int		cmd_proc_int_;
		bool				stat_result_;
	};

	// 内置命令字
	enum build_in_cmd
		{
		cmd_pause,
		cmd_wait,
		cmd_help,
		cmd_report,
		cmd_libfile,
		cmd_logaudio,
		cmd_logfile,
		cmd_system,
		cmd_print,
		cmd_error,
		cmd_logevent,
		cmd_do,
		cmd_define,
		};

	// 内置变量
	enum build_in_var
		{
		var_sn,
		var_round,
		var_thr_id,
		var_time,
		var_i,
		var_line,
		var_label,
		var_file,
		var_toutf8,
		};

	// 一个命令的信息
	class bat_cmd
		{
		public:
		const cmd_info & ci(void) const
			{
			return *ci_;
			}
		const par_arr & pars(void) const
			{
			return pars_;
			}
		const string & should_ret(void) const
			{
			return should_ret_;
			}
		const string & txt(void) const
			{
			return txt_;
			}
		bool is_end(void) const
			{
			return end_;
			}

		bat_cmd(const cmd_info * ci, const par_arr & pars, const string & ret_str)
			: ci_(ci), pars_(pars), should_ret_(ret_str), end_(false)
			{
			}
		bat_cmd(const cmd_info * ci, const par_arr & pars, const string & ret_str, const string & txt)
			: ci_(ci), pars_(pars), should_ret_(ret_str), txt_(txt), end_(true)
			{
			}

		public:
		const cmd_info *	ci_;
		bool				end_;
		par_arr				pars_;
		string				txt_;
		string				should_ret_;
		};
	// 命令字列表
	typedef std::vector<bat_cmd> bat_cmd_arr;

	// 命令字统计信息
	class cmd_record
		{
		public:
			int & succs(void)
				{
				return succ_count_;
				}
			int & fails(void)
				{
				return fail_count_;
				}
			int & passes(void)
				{
				return pass_count_;
				}
			void add_result(bool succ, bool pass)
				{
				succ ? ++succ_count_ : ++fail_count_;
				if ( pass )
					++pass_count_;
				}
			int total(void)
				{
				return succs() + fails();
				}
			string & cmd_name(void)
				{
				return cmd_name_;
				}
			bool operator () (const string & name)
				{
				return sp::strcasecmp(cmd_name_.c_str(), name.c_str()) == 0;
				}
			bool operator == (const cmd_record & cr)
				{
				return sp::strcasecmp(cmd_name_.c_str(), cr.cmd_name_.c_str()) == 0;
				}

			cmd_record(const string & cmd)
				: cmd_name_		(cmd)
				, succ_count_	(0)
				, fail_count_	(0)
				, pass_count_	(0)
				{
				}
			cmd_record(const cmd_record & cr)
				: succ_count_	(0)
				, fail_count_	(0)
				, pass_count_	(0)
				{
				*this = cr;
				}
			cmd_record & operator = (const cmd_record & cr)
				{
				this->succ_count_ = cr.succ_count_;
				this->fail_count_ = cr.fail_count_;
				this->pass_count_ = cr.pass_count_;
				this->cmd_name_	  = cr.cmd_name_;
				return *this;
				}
			
			void make_string(string & desc_str, bool gen_head = false)
				{
				std::basic_stringstream<char_t> stm;
				if ( gen_head )
					{
					const char_t cmd [] = { 'C', 'o', 'm', 'm', 'a', 'n', 'd', '\0', };
					const char_t succ[] = { 'S', 'u', 'c', 'c', 'e', 's', 's', '\0', };
					const char_t fail[] = { 'F', 'a', 'i', 'l', 'e', 'd', '\0', };
					const char_t pass[] = { 'P', 'a', 's', 's', '\0', };
					stm << std::endl
						<< std::setw(32) << cmd
						<< std::setw(8)  << succ
						<< std::setw(8)  << fail
						<< std::setw(8)  << pass
						<< std::endl;
					}

				stm << std::setw(32) << cmd_name_
					<< std::setw(8)  << succ_count_
					<< std::setw(8)  << fail_count_
					<< std::setw(8)  << pass_count_
					<< std::endl;
				desc_str += stm.str();
				}
			
		private:
			int		succ_count_;
			int		fail_count_;
			int		pass_count_;
			string	cmd_name_;
		};
	// 命令字统计
	template<typename char_tt>
	class cmd_stat
		: public std::vector<cmd_record>
		{
		public:
		void add_result(const string & cmd, bool succ, bool pass)
			{
			typename std::vector<cmd_record>::iterator pi = std::find(begin(), end(), cmd);
			if ( pi != end() )
				(*pi).add_result(succ, pass);
			else
				{
				cmd_record cr(cmd);
				cr.add_result(succ, pass);
				push_back(cr);
				}
			succ ? ++total_succ_ : ++total_fail_;
			pass ? ++total_pass_ : 0;
			}
		void make_report(string & report_str)
			{
			for ( size_t i = 0; i < size(); i++ )
				at(i).make_string(report_str, i == 0);

			const char_tt title[] = { 'T', 'o', 't', 'a', 'l', '\0' };
			std::basic_stringstream<char_tt> stm;
			stm << std::endl
				<< std::setw(32) << title
				<< std::setw(8)  << total_succ_
				<< std::setw(8)  << total_fail_
				<< std::setw(8)  << total_pass_
				<< std::endl;

			report_str.append(60, char_tt('-'));
			report_str += stm.str();
			}

		cmd_stat(void)
			: total_succ_	(0)
			, total_fail_	(0)
			, total_pass_	(0)
			{
			}

		private:
			int total_succ_;
			int total_fail_;
			int total_pass_;
		};

	// 派生类需要重载的函数
	virtual const cmd_info * get_cmd_info(void) const
		{
		static const char_t supp_cmds[][32] =
			{
				{ 'p', 'a', 'u', 's', 'e', '\0' },
				{ 'w', 'a', 'i', 't', '\0' },
				{ 'h', 'e', 'l', 'p', '\0' },
				{ 'r', 'e', 'p', 'o', 'r', 't', '\0' },
				{ 'l', 'i', 'b', 'f', 'i', 'l', 'e', '\0' },
				{ 'l', 'o', 'g', 'a', 'u', 'd', 'i', 'o', '\0' },
				{ 'l', 'o', 'g', 'f', 'i', 'l', 'e', '\0' },
				{ 's', 'y', 's', 't', 'e', 'm', '\0' },
				{ 'p', 'r', 'i', 'n', 't', '\0' },
				{ 'e', 'r', 'r', 'o', 'r', '\0' },
				{ 'l', 'o', 'g', 'e', 'v', 'e', 'n', 't', '\0' },
				{ 'd', 'o', '\0', },
				{ 'd', 'e', 'f', 'i', 'n', 'e', '\0' },
			};
		static const cmd_info cmd_map[] =
			{
				{ supp_cmds[cmd_pause],		0,		0,	&asc_cmd_t::on_pause,	false },
				{ supp_cmds[cmd_wait],		0,		0,	&asc_cmd_t::on_wait,	false },
				{ supp_cmds[cmd_help],		&asc_cmd_t::on_help,		0, 0,	false },
				{ supp_cmds[cmd_report],	&asc_cmd_t::on_report,		0, 0,	false },
				{ supp_cmds[cmd_libfile],	&asc_cmd_t::on_libfile,		0, 0,	false },
				{ supp_cmds[cmd_logaudio],	&asc_cmd_t::on_logaudio,	0, 0,	false },
				{ supp_cmds[cmd_logfile],	&asc_cmd_t::on_logfile,		0, 0,	false },
				{ supp_cmds[cmd_system],	&asc_cmd_t::on_system,		0, 0,	false },
				{ supp_cmds[cmd_print],		&asc_cmd_t::on_print,		0, 0,	false },
				{ supp_cmds[cmd_error],		&asc_cmd_t::on_error,		0, 0,	false },
				{ supp_cmds[cmd_logevent],	&asc_cmd_t::on_logevent,	0, 0,	false },
				{ supp_cmds[cmd_do],		&asc_cmd_t::on_do,			0, 0,	false },
				{ supp_cmds[cmd_define],	&asc_cmd_t::on_define,		0, 0,	false },
				{ 0,						0,							0, 0,	false },
			};

		return cmd_map;
		}
	// 合并调用信息
	void get_cmd_info(std::vector<const cmd_info*> & cmds_info)
		{
		const cmd_info * cmd_infos = asc_cmd_t::get_cmd_info();
		for ( int i = 0; cmd_infos[i].cmd_name_; i++ )
			cmds_info.push_back(cmd_infos + i);

		cmd_infos = this->get_cmd_info();
		for ( int i = 0; cmd_infos[i].cmd_name_; i++ )
			cmds_info.push_back(cmd_infos + i);
		}

	virtual int batch_exec(bat_cmd_arr & bat_cmds)
		{
		if ( bat_cmds.empty() )
			return 0;

		// begin of batch
		on_batch_begin();
		for ( int i = 0; i < (int)bat_cmds.size(); i++ )
			{
			if ( bat_stop_ )
				break ;

			int ret = 0;
			bat_cmd & bc = bat_cmds[i];
			// dispatch command
			if ( bc.ci().cmd_proc_int_ )
				{
				int par = bc.pars().empty() ? -1 : sp::strtoi(bc.pars().first_value());
				ret = (dynamic_cast<T*>(this)->*bc.ci().cmd_proc_int_)(par);
				}
			else if ( bc.ci().cmd_proc_beg_ && !bc.is_end() )
				{
				ret = (dynamic_cast<T*>(this)->*bc.ci().cmd_proc_beg_)(bc.pars());
				}
			if ( bc.ci().cmd_proc_end_ && bc.is_end() )
				{
				ret = (dynamic_cast<T*>(this)->*bc.ci().cmd_proc_end_)(bc.pars(), bc.txt());
				}

			if ( bc.ci().stat_result_ )
				cmd_stat_.add_result(bc.ci().cmd_name_, ret == 0, verify_return(bc.ci().cmd_name_, last_rets_, ret));
			}
		on_batch_end();
		return 0;
		}

	// 获得变量的名字和参数表
	int parse_variable(const string & var_str, string & var, par_arr & pars)
		{
		typename string::size_type pos = var_str.find('(');
		if ( pos != std::string::npos )
			{
			// variable name
			var = var_str.substr(0, pos);
			sp::trim_str(var);

			// parse params
			const char_t trim[] = { '(', ')', '\0' };
			string par_str = var_str.substr(pos);
			sp::trim_str(par_str, trim);
			if ( !par_str.empty() )
				pars.parse(par_str.c_str());
			return 0;
			}

		return -1;
		}

protected:
	// 变量
	virtual int on_variable(const char_t * cmd_name, const char_t * var_name, const char_t *& result, size_t & len)
		{
		int ret = on_variable(cmd_name, var_name, var_result_);
		result = var_result_.c_str();
		len    = var_result_.size();
		return ret;
		}

	// 内置支持的命令字
	virtual int on_pause(int time_out)
		{
		dslog_tracer(asc_cmd_t::on_pause);
		if ( time_out == 0 || time_out == -1 )
			{
			dslog_info("Paused, any key to continue...\n");
			#ifdef WIN32
			system("pause");
			#else // Linux
			printf("Paused, any key to continue...\n");
			getchar();
			#endif // OS
			}
		else
			{
			dslog_info("Sleep %d ms to resume.\n", time_out);
			#ifdef WIN32
			Sleep(time_out);
			#else
			usleep(time_out * 1000);
			#endif
			}
		return 0;
		}
	virtual int on_wait (int time_out)
		{
		dslog_tracer(asc_cmd_t::on_wait);
		return 0;
		}

	virtual int on_logfile (const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_logfile);
		if ( !pars.empty() )
			log_file_ = pars.first_value();

		dslog_verbose("asc_cmd_t::on_logfile log file is %s.", sp::tochar(log_file_).c_str());
		return 0;
		}
	virtual int on_libfile (const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_libfile);
		return 0;
		}
	virtual int on_logaudio(const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_logaudio);
		if ( !pars.empty() )
			audio_file_ = pars.first_value();

		dslog_verbose("asc_cmd_t::on_logaudio audio file is %s.", sp::tochar(audio_file_).c_str());
		return 0;
		}
	virtual int on_help(const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_help);

		std::basic_stringstream<char_t> out;
		const char_t head[] = { ' ', ' ', 'h', 'e', 'l', 'p', ' ', '\0' };
		const char_t crlf[] = { '\r', '\n', '\0' };

		string help_str;
		help_str.append(30, char_t('-'));
		help_str += crlf;
		help_str += head;
		help_str += name();
		help_str += crlf;
		help_str.append(30, char_t('-'));

		out.setf(std::ios::left);
		out << std::endl << help_str << std::endl;
		for ( int i = 0; i < (int)cmd_info_.size(); i++ )
			{
			const cmd_info & ci = *cmd_info_[i];
			out << std::setw(20) << char_t('\t') << ci.cmd_name_
				<< std::endl;
			}
		if ( dslog_inst )
			dslog_inst->log_msg_i(0, lgl_info, lgs_default, out.str().c_str());
		return 0;
		}
	virtual int on_report(const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_report);

		string report_str;
		cmd_stat_.make_report(report_str);
		
		string report_file(pars.first_value_string());
		if ( !report_file.empty() )
			{
			#ifdef WIN32
			std::basic_ofstream<char_t, std::char_traits<char_t> >
				rf(report_file.c_str(), std::ios_base::app);
			#else // LNX
			std::basic_ofstream<char_t, std::char_traits<char_t> >
				rf(sp::tochar(report_file).c_str(), std::ios_base::app);
			#endif // OS
			if ( rf )
				rf << report_str;
			}
	
		const char head[] = 
			"=============================================================\n"
			"Report the test results:\n"
			"=============================================================";
		const char tail[] =
			"=============================================================\n";

		if ( dslog_inst )
			{
			dslog_inst->log_msg(head);
			dslog_inst->log_msg(report_str.c_str());
			dslog_inst->log_msg(tail);
			}
		return 0;
		}
	virtual int on_system(const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_system);
		
		string cmd = pars.first_value_string();

		dslog_verbose("asc_cmd_t::on_system execute cmd begin.");
		dslog_verbose(cmd.c_str());
		if ( cmd.length() )
			sp::system(cmd.c_str());
		dslog_verbose("asc_cmd_t::on_system execute cmd end.");
		return 0;
		}
	virtual int on_print(const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_print);
		for ( int i = 0; i < (int)pars.size(); i++ )
			wprintf(L"%s\n", sp::towchar(pars.first_value()).c_str());
		return 0;
		}
	virtual int on_error(const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_error);
		if ( pars.empty() )
			dslog_error("asc_cmd_t::on_error must have parameters, i.e. \"error goto end\".");
		
		string cmd = pars.first_par().first_value_string();
		string act = pars.first_par().second_value_string();
		if ( last_ret_ != 0 && !act.empty() )
			{
			goto_cmd_ = act;
			dslog_info("asc_cmd_t::on_error will %s %s.", sp::tochar(cmd).c_str(), sp::tochar(act).c_str());
			}

		return 0;
		}
	virtual int on_logevent(const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_error);
		if ( pars.empty() )
			dslog_error("asc_cmd_t::on_logevent must have parameters, i.e. \"logevent \"Hello world\"\".");
		
		if ( !pars.empty() )
			dslog_crit(pars.first_value());
		return 0;
		}
	virtual int on_do(const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_do);
		if ( pars.empty() )
			dslog_error("asc_cmd_t::on_do must have parameters, i.e. \"do clear\".");
		return 0;
		}
	virtual int on_define(const par_arr & pars)
		{
		dslog_tracer(asc_cmd_t::on_define);
		if ( pars.empty() )
			{
			dslog_error("asc_cmd_t::on_define must have parameters, i.e. \"define name=value\".");
			return 1;
			}
		user_vars_.insert(std::make_pair(pars.first_par().par_name(), pars.first_par().par_value()));
		return 0;
		}

// 派生类可以重载的函数
protected:
	// 校验返回值
	virtual bool verify_return(const string & cmd, const string & ret_str, int ret_val)
		{
		if ( ret_str.empty() )
			return ret_val == 0;

		int should_ret = sp::strtoui(ret_str.c_str());
		if ( should_ret == 0 )
			{
			if ( ret_val == 0 )
				return true;
			const char_t * codes = parse_error_code(ret_val);
			if ( codes )
				return sp::strcasecmp(codes, ret_str.c_str()) == 0;
			}
		return should_ret == ret_val;
		}
	// 解析错误码
	virtual const char_t * parse_error_code(int code)
		{
		return 0;
		}

	// 获得当前的服务ID
	virtual unsigned int svc_id(void) const
		{
		return 0;
		}
	// 获得当前的测试轮数
	virtual unsigned int svc_round(void) const
		{
		return 0;
		}
	// 获得测试的标记
	virtual const std::string & label(void) const
		{
		static const std::string empty;
		return empty;
		}

	// 变量解析
	virtual int on_variable(const char_t * cmd_name, const char_t * var_name, string & result)
		{
		// parse variable
		par_arr pars;
		string var_str;
		parse_variable(var_name, var_str, pars);

		// 支持的内置变量
		static const char_t supp_vars[][32] =
			{
				{ 's', 'n', '\0' },
				{ 'r', 'o', 'u', 'n', 'd', '\0' },
				{ 't', 'h', 'r', '\0' },
				{ 't', 'i', 'm', 'e', '\0' },
				{ 'i', '\0' },
				{ 'l', 'i', 'n', 'e', '\0' },
				{ 'l', 'a', 'b', 'e', 'l', '\0' },
				{ 'f', 'i', 'l', 'e', '\0', },
				{ 't', 'o', 'u', 't', 'f', '8', '\0', },
			};

		int idx = sp::search_strarr(var_str.c_str(), supp_vars, false);
		switch ( idx )
			{
			case var_sn:
				{
				char_t fmt[16] = { '%', '0', '4', 'd', '\0' };
				if  ( !pars.empty() )
					sp::strsncpy(fmt, pars.first_par_str_value(), sizeof fmt);

				sp::itostr(svc_id(), result, fmt);
				}
				break;
			case var_round:
				{
				char_t fmt[16] = { '%', '0', '4', 'd', '\0' };
				if  ( !pars.empty() )
					sp::strsncpy(fmt, pars.first_par_str_value(), sizeof fmt);

				sp::itostr(svc_round(), result, fmt);
				}
				break;
			case var_i:
				{
				int value = parser()->loop_cursor();
				if ( pars.first_par_str_value() )
					value += sp::strtoi(pars.first_par_str_value());
				sp::itostr(value, result, pars.second_par_str_value());
				}
				break;
			case var_thr_id:
				{
				sp::itostr(sp::thr_self(), result);
				}
				break;
			case var_time:
				{
				char_t times[64]; times[0] = 0;
				result = sp::cur_time(times, true, false, false);
				}
				break;
			case var_line:
				{
				sp::itostr(parser()->line_no(), result);
				}
				break;
			case var_label:
				{
				sp::strcopy(result, label().c_str());
				}
				break;
			case var_file:
				if ( pars.first_par_str_value() )
				{
				std::string fname = sp::tochar(pars.first_par_str_value());
				size_t flen = sp::get_file_size(fname.c_str());
				if ( flen > 0 )
					{
					char* data = new char[flen];
					sp::read_bin_file(fname.c_str(), data, flen);
					result.assign((char_t*)data, flen / sizeof(char_t));
					delete [] data;
					}
					else
					{
					dslog_warn("asc_cmd_t::on_variable the length of the file is 0, file : %s", fname.c_str());
					}
				}
				break;
			case var_toutf8:
				{
				if ( pars.first_par_str_value() )
					{
					std::string utf_str = sp::toutf8(pars.first_par_str_value());
					result.assign((char_t*)utf_str.c_str(), utf_str.size());
					}
				}
				break;
			default:
				{
				typename str_map::iterator pi = user_vars_.find(var_str);
				result = pi != user_vars_.end() ? pi->second : var_name;
				}
				break;
			}
		
		return 0;
		}

private:
	// command info array
	typedef std::vector<const cmd_info*> cmdi_arr;
	cmdi_arr		cmd_info_;

	// batch commands
	bat_cmd_arr		bat_cmds_;
	bool			bat_mode_;
	bool			bat_stop_;

	// command statistic
	cmd_stat<char_t> cmd_stat_;

	// last cmd pars
	par_arr			last_pars_;
	string			last_rets_;
	int				last_ret_;

	// 日志文件
	string			log_file_;
	// 音频文件
	string			audio_file_;

	// 定位的命令字
	string			goto_cmd_;

	// 当前的变量结果
	string			var_result_;

	// 自定义变量映射表
	str_map			user_vars_;
	};

// 宽字符版
template <class T>
class wasc_cmd_t
	: public asc_cmd_t<T, sc::wasc_sink_i, wchar_t, sc::wasc_parser_i>
	{
	};


/** 
 * @brief 	get_scp_subject
 *  
 *   直接获取一个脚本的主题
 *  
 * @author	jdyu
 * @date	2007-7-20
 * @return	inline std::basic_string<char_t> 
 * @param	const char_t * scp_file	- [in] 指定的脚本文件
 * @see		
 */
template <typename char_t>
inline std::basic_string<char_t> get_scp_subject(const char_t * scp_file)
	{
	typedef std::basic_string<char_t> string;

	#ifdef WIN32
	std::basic_ifstream<char_t, std::char_traits<char_t> > fs(scp_file);
	#else // LNX
	std::basic_ifstream<char_t, std::char_traits<char_t> > fs(sp::tochar(scp_file).c_str());
	#endif // OS
	if ( fs.fail() )
		{
		dslog_error("Cannot open scp_file %s, errno %d!", scp_file, errno);
		return string();
		}

	#ifdef WIN32
	fs.imbue(std::locale(""));
	#endif // WIN32

	// read all file
	char_t line[400];
	for ( int i = 0; i < 50 && !fs.eof(); i++ )
		{
		line[0] = 0;
		fs.getline(line, SP_COUNTOF(line));
		if ( fs.fail() )
			break;
		int len = sp::trim_str(line);

		// 空行、注释行
		if ( line[0] == 0 ||  line[0] == '#' || line[0] == ';' )
			continue ;

		std::vector<string> pars;
		const char_t splt[] = { ' ', '\t', '\0' };
		sp::split_str(line, pars, splt, true, true, true);

		const char_t sub_name[] = { 's', 'u', 'b', 'j', 'e', 'c', 't', '\0' };
		if ( pars.size() > 1
			&& sp::strcasecmp(pars.front().c_str(), sub_name) == 0 )
			return pars.at(1);
		else
			break ;
		}
	fs.close();

	return string();
	}

// ============================================================================
// 方便使用的定义
// ============================================================================

/// 一个用于定义名字的类，宽字符版本
template <wchar_t first = L'\0', wchar_t second = L'\0', wchar_t third = L'\0', wchar_t fourth = L'\0', wchar_t fifth = L'\0', wchar_t sixth = L'\0', wchar_t seventh = L'\0'>
class wasc_name_t
	{
	public:
	const wchar_t * operator() (void)
		{
		static const wchar_t name[] = 
			{ first, second, third, fourth, fifth, sixth, seventh, L'\0' };
		return name;
		}
	};

// 窄字符版本
template <char first = '\0', char second = '\0', char third = '\0', char fourth = '\0', char fifth = '\0', char sixth = '\0', char seventh = '\0'>
class asc_name_t
	{
	public:
	const char * operator() (void)
		{
		static const char name[] = 
			{ first, second, third, fourth, fifth, sixth, seventh, '\0' };
		return name;
		}
	};

/** 
 * @class	asc_session_t
 * 
 * @brief	把解析器和响应器捆绑在一起
 * 
 *  提供一个解析器和响应器的绑定实现
 * 
 * @author	jdyu
 * @date	2007-7-18
 * 
 * @see		
 */
template <class PARSER, class SINK, class char_t = char, class NAME = asc_name_t<> >
class asc_session_t
	: public PARSER
	{
public:
	typedef std::basic_string<char_t> string;

	asc_session_t(const string & asc_file, const char_t * scp_sub, bool bat_mode = false)
		: scp_file_		(asc_file)
		{
		parser_sub_ = NAME()();
		sink_.name(parser_sub_.c_str());
		sink_.bat_mode(bat_mode);

		scp_sub_ = get_scp_subject(asc_file.c_str());
		if ( scp_sub_.empty() && scp_sub != 0 )
			scp_sub_ = scp_sub;

		add_parse_sink(&sink_);
		}

	asc_session_t(bool bat_mode = false)
		{
		parser_sub_ = NAME()();
		sink_.name(parser_sub_.c_str());
		sink_.bat_mode(bat_mode);
		add_parse_sink(&sink_);
		}

	asc_session_t & operator = (const asc_session_t & ssn)
		{
		parser_sub_ = ssn.parser_sub_;
		scp_sub_ = ssn.scp_sub_;
		scp_file_ = ssn.scp_file_;
		return *this;
		}
	asc_session_t(const asc_session_t & ssn)
		{
		*this = ssn;
		add_parse_sink(&sink_);
		}

	virtual ~asc_session_t(void)
		{
		remove_parse_sink(&sink_);
		}

	const string & get_subject(void) const
		{
		return parser_sub_;
		}
	const string & scp_subject(void) const
		{
		return scp_sub_;
		}

	SINK & sink(void)
		{
		return sink_;
		}

	int parse_file(const char_t * asc_file = 0)
		{
		if ( sink_.bat_mode() &&
			sp::strcasecmp(asc_file, scp_file_.c_str()) == 0 )
			return 0;

		if ( asc_file )
			scp_file_ = asc_file;
		if ( scp_sub_.empty() )
			scp_sub_ = get_scp_subject(scp_file_.c_str());

		if ( scp_sub_.empty() ||
			sp::strcasecmp(scp_sub_.c_str(), parser_sub_.c_str()) == 0 )
			return PARSER::parse_file(scp_file_.c_str());

		return -1;
		}

	int batch_exec(void)
		{
		return sink_.batch_exec();
		}
	void batch_stop(void)
		{
		sink_.batch_stop();
		}

private:
	SINK		sink_;
	string		parser_sub_;
	string		scp_sub_;
	string		scp_file_;
	};

/// 单体
template<class T>
class asc_singleton
	{
public:
	static T & instance(void)
		{
		static T the_inst;
		return the_inst;
		}
	virtual ~asc_singleton(void)
		{
		}
	};

} /* namespace sc */

// Sun OS pack
#ifndef WIN32
#pragma pack(pop)
#endif // sun

#endif /* __ASC_PARSER_H__ */
