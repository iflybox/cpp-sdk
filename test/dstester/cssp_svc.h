#ifndef _IFLYBPX_SVC_H__
#define _IFLYBPX_SVC_H__

#if _MSC_VER > 1000
# pragma once
#endif

#ifndef WIN32
# include <string.h>
# include <stdlib.h>
# include <netdb.h>
# include <sys/socket.h>
#endif

#include "utils/sutils.h"
#include "utils/sulib.h"
#include "asc_parser.h"
#include "thr_mngr.h"
#include "dstester_cfg.h"
#include "cssp.h"
//#include "iconv.h"

#ifdef WIN32
const char cssp_lib_name[] = "cssp.dll";
#else
const char cssp_lib_name[] = "cssp.so";
#endif

class cssp_unit
	: public run_unit_t<ru_filter_scp, char>
{
public:
	// parent
	typedef run_unit_t<ru_filter_scp, char> parent;
	
	struct match_resource
	{
		bool operator () (const char * file)
		{
			const char * res_exts[] =
			{
				"*",
			};

			return sp::search_strarr_case(sp::pathname_to_extname(file).c_str(), res_exts) >= 0;
		}
	};

	cssp_unit(const char * uri = 0)
	{
		if (match_unit(uri))
		{
			test_file_ = uri;
			scp_file_ = uri;
		}	

		if ( !test_file_.empty() )
		{
			if ( res_file_.empty() && !cfg_mngr().resource_dir().empty() )
				res_file_ = cfg_mngr().resource_dir().c_str();
		}

	}
	~cssp_unit(void)
	{
		/*if ( wave_data_ )
			delete [] wave_data_;*/
	}
	operator bool(void) const
	{
		bool b = cfg_mngr().file_mode();
		return cfg_mngr().file_mode();
	}

	// 匹配一个单元
	bool match_unit(const char * uri)
	{
		sp::str_arr ress;
		// 查找所有的语法文件
		const char* res_uri = cfg_mngr().resource_dir().c_str();
		sp::get_file_list(res_uri, ress, "*.*", false);

		// 复制
		for ( size_t i = 0; i < ress.size(); i++ )
			res_files_.push_back(ress[i]);
		
		if ( cfg_mngr().file_mode() )
			return parent::match_unit(uri);
		if ( uri == 0 )
			return false;

		// 如果是目录模式，那么要确定这个目录下是否存在脚本文件
		sp::str_arr scps;
		sp::get_file_list(uri, scps, "*.scp", false);
		if ( scps.empty() )
		{
			// 还可以使用配置文件里面的脚本文件
			if ( cfg_mngr().scp_file().empty() )
				return false;
			scps.push_back(cfg_mngr().scp_file());
		}
		this->scp_file_ = scps.front();

		return true;
	}

	// 预处理函数
	int prepare(void)
	{
		return 0;
	}

public:
	// 获得资源文件
	const char * resource_file(size_t cursor) const
	{
		return res_files_[cursor % res_files_.size()].c_str();
	}
	
protected:
private:
	// 资源文件
	std::string		res_file_;
	// 目录模式下的列表
	sp::str_arr		res_files_;
};

class cssp_lib
	: public sp::su_lib<>
{
public:
	cssp_lib(const char * lib = cssp_lib_name)
		: sp::su_lib<>(lib)
	{
	}

protected:
	virtual int load_procs(void)
	{
		su_proc_load(cssp_init_result)
		su_proc_load(cssp_result_statuscode)
		su_proc_load(cssp_result_detail)
		su_proc_load(cssp_release_result)
		su_proc_load(cssp_create_instance)
		su_proc_load(cssp_release_instance)

//		su_proc_load(cssp_delete_container_ifempty)
		su_proc_load(cssp_list_object)
		su_proc_load(cssp_count_object)
		su_proc_load(cssp_count_bytesused)

		su_proc_load(cssp_object_exist)
		su_proc_load(cssp_put_object_fromdisk_callback)
		su_proc_load(cssp_put_object)
		su_proc_load(cssp_get_object_todisk_callback)
		su_proc_load(cssp_get_object)
		su_proc_load(cssp_remove_object)
		su_proc_load(cssp_copy_object)
		su_proc_load(cssp_move_object)
		su_proc_load(cssp_set_objectmeta)
		su_proc_load(cssp_get_objectmeta)

		su_proc_load(cssp_objectmeta_contentlength)
		su_proc_load(cssp_objectmeta_etag)
		su_proc_load(cssp_objectmeta_lastmodify)

		su_proc_load(cssp_multipart_upload_init)
		su_proc_load(cssp_multipart_upload_part)
		su_proc_load(cssp_multipart_upload_list_parts)
		su_proc_load(cssp_multipart_upload_complete)
		su_proc_load(cssp_multipart_upload_abort)


		su_proc_load(cssp_objectmeta_create)
		su_proc_load(cssp_objectmeta_release)

		su_proc_load(cssp_metadata_ownername)
		su_proc_load(cssp_metadata_get)
		su_proc_load(cssp_metadata_set)
		su_proc_load(cssp_metadata_count)
		
		return 0;
	}

private:
	su_proc_decl(cssp_init_result)
	su_proc_decl(cssp_result_statuscode)
	su_proc_decl(cssp_result_detail)
	su_proc_decl(cssp_release_result)
	su_proc_decl(cssp_create_instance)
	su_proc_decl(cssp_release_instance)

//	su_proc_decl(cssp_delete_container_ifempty)
	su_proc_decl(cssp_list_object)
	su_proc_decl(cssp_count_object)
	su_proc_decl(cssp_count_bytesused)

	su_proc_decl(cssp_object_exist)
	su_proc_decl(cssp_put_object_fromdisk_callback)
	su_proc_decl(cssp_put_object)
	su_proc_decl(cssp_get_object_todisk_callback)
	su_proc_decl(cssp_get_object)
	su_proc_decl(cssp_remove_object)
	su_proc_decl(cssp_copy_object)
	su_proc_decl(cssp_move_object)
	su_proc_decl(cssp_set_objectmeta)
	su_proc_decl(cssp_get_objectmeta)
	su_proc_decl(cssp_objectmeta_contentlength)
	su_proc_decl(cssp_objectmeta_etag)
	su_proc_decl(cssp_objectmeta_lastmodify)

	su_proc_decl(cssp_multipart_upload_init)
	su_proc_decl(cssp_multipart_upload_part)
	su_proc_decl(cssp_multipart_upload_list_parts)
	su_proc_decl(cssp_multipart_upload_complete)
	su_proc_decl(cssp_multipart_upload_abort)

	su_proc_decl(cssp_objectmeta_create)
	su_proc_decl(cssp_objectmeta_release)

	su_proc_decl(cssp_metadata_ownername)
	su_proc_decl(cssp_metadata_get)
	su_proc_decl(cssp_metadata_set)
	su_proc_decl(cssp_metadata_count)
		
};

class ibox_mngr
	: public svc_mngr<ibox_mngr>
{
protected:
	// 必须实现的初始化
	int on_initialize(const sp::str_arr &libs)
	{
		int ret = 0;
		if(libs.empty())
		{
			dslog_warn("ibox_mngr::on_initiallize | no lib file specified, you can load lib in scp file.");
		}
		else
		{
			// 加载dll / so
			ret = cssp_lib_.open(libs.front().c_str());
			if(ret != 0)
			{
				dslog_error_assert(0, ("ibox_mngr::on_initiallize | can not load lib %ls.", libs.front().c_str()));
				return ret;
			}
			else
			{
				// 调用初始化系统接口
				// 读取cssp相关配置项
				// wirte_localfile_
				sp::inisec isec_cssp(cfg_mngr().cfg_file().c_str(), "cssp");
				write_localfile_ = false;
				isec_cssp.get_bool_value("write_localfile", write_localfile_);
               
				std::string endpoint, accessKeyId, accessKeySecret;
				int endpoint_type;
				//在dstester的配置中获取
				isec_cssp.get_str_value("endpoint", endpoint);
				isec_cssp.get_str_value("access_key_id", accessKeyId);
				isec_cssp.get_str_value("access_key_secret", accessKeySecret);
				
				
				dslog_perf_n(cssp_create_instance);
		
				CSSPResult result = cssp_lib_.cssp_init_result()();
				cssp_handle_ = cssp_lib_.cssp_create_instance()(endpoint.c_str(), accessKeyId.c_str(), accessKeySecret.c_str(), result);
				
				double nsec = dslog_perf_stop();
				proc_resp_mngr().add_nsec("cssp_create_instance", nsec);
                
				if (cssp_handle_ == NULL)
				{
					proc_resp_mngr().add_record("cssp_create_instance_failed");
					dslog_error("ibox_mngr | cssp_create_instance failed, result = %d, error msg:%s.", cssp_lib_.cssp_result_statuscode()(result), cssp_lib_.cssp_result_detail()(result));
					cssp_lib_.cssp_release_result()(result);
					return -1;
				}
				cssp_lib_.cssp_release_result()(result);
				dslog_crit("ibox_mngr | cssp_create_instance success, time %4.3f.", nsec * 1e-9);
				proc_resp_mngr().add_record("cssp_create_instance_success");


				return ret;
			}
		}
		return 0;
	}

	int on_uninitialize(void)
	{
		// 逆初始化
		int error_code = 0;
		double nsec = 0.;


		dslog_perf_nsec(cssp_release_instance, nsec);
		int ret = cssp_lib_.cssp_release_instance()(cssp_handle_);
		proc_resp_mngr().add_nsec("cssp_release_instance", dslog_perf_stop());
		
		if (ret)
		{
			proc_resp_mngr().add_record("cssp_release_instance_failed");
			dslog_error("ibox_mngr | cssp_release_instance failed, error = %d", ret);
			return error_code;
		}
		
		dslog_crit("ibox_mngr | cssp_release_instance success, time %4.3f.", nsec * 1e-9);
		proc_resp_mngr().add_record("cssp_release_instance_success");

		cssp_lib_.close();

		return 0;
	}

public:
	ibox_mngr(void)
	{
	}

	cssp_lib & cssp(void)
	{
		return cssp_lib_;
	}
	CSSPHandle cssp_handle(void){
		return cssp_handle_;
	}
	virtual const char * default_scp(void) const 
	{
		return cfg_mngr().scp_file().c_str();
	}

	virtual bool has_units(void) const
	{
		std::string s_mode_ = cfg_mngr().scp_mode();
		if (s_mode_ == "dir")
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool write_localfile()
	{
		return write_localfile_;
	}
private:
	cssp_lib		cssp_lib_;
	CSSPHandle		cssp_handle_;
	bool			write_localfile_;
};



class cssp_svc
	: public sc::asc_cmd_t<cssp_svc>
	, public svc_bind_unit<cssp_unit>
{
#ifdef _WIN32
	typedef int (__thiscall cssp_svc::*Proc_Metadata)(MetadataHandle);
#else
	typedef int (cssp_svc::*Proc_Metadata)(MetadataHandle);
#endif
public:
	cssp_svc(void);
	virtual ~cssp_svc(void);

	virtual int on_begin();
	virtual int on_end();

protected:
	asc_cmd_entry_begin(cssp_svc)
		//asc_cmd_entry_pars(cssp_svc, cssp_delete_container_ifempty)
		asc_cmd_entry_pars(cssp_svc, cssp_object_exist)
		asc_cmd_entry_pars(cssp_svc, cssp_put_object)
        asc_cmd_entry_pars(cssp_svc, cssp_get_object)
		asc_cmd_entry_pars(cssp_svc, cssp_remove_object)
		asc_cmd_entry_pars(cssp_svc, cssp_copy_object)
		asc_cmd_entry_pars(cssp_svc, cssp_move_object)
		asc_cmd_entry_pars(cssp_svc, cssp_list_object)
		asc_cmd_entry_pars(cssp_svc, cssp_objectmeta_create)
		asc_cmd_entry_pars(cssp_svc, cssp_objectmeta_release)
		asc_cmd_entry_pars(cssp_svc, cssp_get_objectmeta)
		asc_cmd_entry_pars(cssp_svc, cssp_count_object)
		asc_cmd_entry_pars(cssp_svc, cssp_count_bytesused)
		asc_cmd_entry_pars(cssp_svc, cssp_objectmeta_contentlength)
		asc_cmd_entry_pars(cssp_svc, cssp_objectmeta_etag)
		asc_cmd_entry_pars(cssp_svc, cssp_objectmeta_lastmodify)
		asc_cmd_entry_pars(cssp_svc, cssp_multipart_upload_init)
		asc_cmd_entry_pars(cssp_svc, cssp_multipart_upload_part)
		asc_cmd_entry_pars(cssp_svc, cssp_multipart_upload_list_parts)
		asc_cmd_entry_pars(cssp_svc, cssp_multipart_upload_complete)
		asc_cmd_entry_pars(cssp_svc, cssp_multipart_upload_abort)
		asc_cmd_entry_pars(cssp_svc, cssp_metadata_set)
		asc_cmd_entry_pars(cssp_svc, cssp_set_objectmeta)
		asc_cmd_entry_pars(cssp_svc, cssp_metadata_get)
		asc_cmd_entry_pars(cssp_svc, loop_metadata)
	asc_cmd_entry_end();
protected:
	//virtual int on_cssp_delete_container_ifempty(const sc::par_arr& pars);
	virtual int on_cssp_object_exist(const sc::par_arr& pars);
	virtual int on_cssp_put_object(const sc::par_arr& pars);
    virtual int on_cssp_get_object(const sc::par_arr& pars);
	virtual int on_cssp_remove_object(const sc::par_arr& pars);
	virtual int on_cssp_copy_object(const sc::par_arr& pars);
	virtual int on_cssp_move_object(const sc::par_arr& pars);
	virtual int on_cssp_list_object(const sc::par_arr& pars);
	virtual int on_cssp_objectmeta_create(const sc::par_arr& pars);
	virtual int on_cssp_objectmeta_release(const sc::par_arr& pars);
	virtual int on_cssp_get_objectmeta(const sc::par_arr& pars);
	virtual int on_cssp_count_object(const sc::par_arr& pars);
	virtual int on_cssp_count_bytesused(const sc::par_arr& pars);
	virtual int on_cssp_objectmeta_contentlength(const sc::par_arr& pars);
	virtual int on_cssp_objectmeta_etag(const sc::par_arr& pars);
	virtual int on_cssp_objectmeta_lastmodify(const sc::par_arr& pars);
	virtual int on_cssp_multipart_upload_init(const sc::par_arr& pars);
	virtual int on_cssp_multipart_upload_part(const sc::par_arr& pars);
	virtual int on_cssp_multipart_upload_list_parts(const sc::par_arr& pars);
	virtual int on_cssp_multipart_upload_complete(const sc::par_arr& pars);
	virtual int on_cssp_multipart_upload_abort(const sc::par_arr& pars);
	virtual int on_cssp_metadata_set(const sc::par_arr& pars);
	virtual int on_cssp_metadata_get(const sc::par_arr& pars);
	virtual int on_cssp_set_objectmeta(const sc::par_arr& pars);
	//自定义辅助工具
	virtual int on_loop_metadata(const sc::par_arr& pars);
	virtual int impl_cssp_count_object();
	virtual int impl_cssp_count_bytesused();
	virtual int impl_cssp_objectmeta_contentlength(MetadataHandle handle);
	virtual int impl_cssp_objectmeta_etag(MetadataHandle handle);
	virtual int impl_cssp_objectmeta_lastmodify(MetadataHandle handle);

	cssp_lib & get_cssp_lib(void);

private:
	cssp_lib &	cssp_lib_;

	CSSPHandle		cssp_handle_;
	
	char* buf_value;

	bool				write_localfile_;
	int					container_meta_cnt_;
	ObjectMetaHandle*	object_metas_;
	int					object_meta_cnt_;
	ObjectMetaHandle	last_used_objectmeta_;
	UploadId			last_upload_id_;
	std::map<std::string, Proc_Metadata>	impl_procs;
};




typedef sc::asc_session_t<sc::asc_parser, svc_sink_impl<cssp_svc>, char, 
	sc::asc_name_t<'c', 's', 's', 'p'> >
	cssp_svc_parser;

typedef thr_admin_parser<ibox_mngr, cssp_svc_parser, cssp_unit> cssp_mngr;

#endif	/* _IFLYBPX_SVC_H__ */
