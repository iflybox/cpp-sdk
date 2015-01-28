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
#include "iflybox.h"
//#include "iconv.h"

#ifdef WIN32
const char iflybox_lib_name[] = "iflybox.dll";
#else
const char iflybox_lib_name[] = "iflybox.so";
#endif

class iflybox_unit
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

	iflybox_unit(const char * uri = 0)
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
	~iflybox_unit(void)
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

class iflybox_lib
	: public sp::su_lib<>
{
public:
	iflybox_lib(const char * lib = iflybox_lib_name)
		: sp::su_lib<>(lib)
	{
	}

protected:
	virtual int load_procs(void)
	{
		su_proc_load(iflybox_init_result)
		su_proc_load(iflybox_result_statuscode)
		su_proc_load(iflybox_result_detail)
		su_proc_load(iflybox_release_result)
		su_proc_load(iflybox_create_instance)
		su_proc_load(iflybox_reauth_account)
		su_proc_load(iflybox_release_instance)
		su_proc_load(iflybox_list_buckets)
		su_proc_load(iflybox_set_accountmeta)
		su_proc_load(iflybox_get_accountmeta)
		su_proc_load(iflybox_remove_accountmeta)
		su_proc_load(iflybox_accountmeta_bucketcount)
		su_proc_load(iflybox_accountmeta_bytesused)

		su_proc_load(iflybox_bucket_exist)
		su_proc_load(iflybox_create_bucket)
		su_proc_load(iflybox_delete_bucket_ifempty)
		su_proc_load(iflybox_list_object)
		su_proc_load(iflybox_set_bucketmeta)
		su_proc_load(iflybox_get_bucketmeta)
		su_proc_load(iflybox_remove_bucketmeta)
		su_proc_load(iflybox_bucketmeta_objectcount)
		su_proc_load(iflybox_bucketmeta_bytesused)

		su_proc_load(iflybox_object_exist)
		su_proc_load(iflybox_put_object_fromdisk_callback)
		su_proc_load(iflybox_put_object)
		su_proc_load(iflybox_get_object_todisk_callback)
		su_proc_load(iflybox_get_object)
		su_proc_load(iflybox_remove_object)
		su_proc_load(iflybox_copy_object)
		su_proc_load(iflybox_move_object)
		su_proc_load(iflybox_set_objectmeta)
		su_proc_load(iflybox_get_objectmeta)
//		su_proc_load(iflybox_remove_objectmeta)
		su_proc_load(iflybox_objectmeta_is_pseudodirectory)
		su_proc_load(iflybox_objectmeta_contentlength)
		su_proc_load(iflybox_objectmeta_etag)
		su_proc_load(iflybox_objectmeta_lastmodify)

		su_proc_load(iflybox_multipart_upload_init)
		su_proc_load(iflybox_multipart_upload_part)
		su_proc_load(iflybox_multipart_upload_list_parts)
		su_proc_load(iflybox_multipart_upload_complete)
		su_proc_load(iflybox_multipart_upload_abort)

		su_proc_load(iflybox_accountmeta_create)
		su_proc_load(iflybox_accountmeta_release)
		su_proc_load(iflybox_bucketmeta_create)
		su_proc_load(iflybox_bucketmeta_release)
		su_proc_load(iflybox_objectmeta_create)
		su_proc_load(iflybox_objectmeta_release)

		su_proc_load(iflybox_metadata_ownername)
		su_proc_load(iflybox_metadata_get)
		su_proc_load(iflybox_metadata_set)
		su_proc_load(iflybox_metadata_count)
		
		return 0;
	}

private:
	su_proc_decl(iflybox_init_result)
	su_proc_decl(iflybox_result_statuscode)
	su_proc_decl(iflybox_result_detail)
	su_proc_decl(iflybox_release_result)
	su_proc_decl(iflybox_create_instance)
	su_proc_decl(iflybox_reauth_account)
	su_proc_decl(iflybox_release_instance)
	su_proc_decl(iflybox_list_buckets)
	su_proc_decl(iflybox_set_accountmeta)
	su_proc_decl(iflybox_get_accountmeta)
	su_proc_decl(iflybox_remove_accountmeta)
	su_proc_decl(iflybox_accountmeta_bucketcount)
	su_proc_decl(iflybox_accountmeta_bytesused)

	su_proc_decl(iflybox_bucket_exist)
	su_proc_decl(iflybox_create_bucket)
	su_proc_decl(iflybox_delete_bucket_ifempty)
	su_proc_decl(iflybox_list_object)
	su_proc_decl(iflybox_set_bucketmeta)
	su_proc_decl(iflybox_get_bucketmeta)
	su_proc_decl(iflybox_remove_bucketmeta)
	su_proc_decl(iflybox_bucketmeta_objectcount)
	su_proc_decl(iflybox_bucketmeta_bytesused)

	su_proc_decl(iflybox_object_exist)
	su_proc_decl(iflybox_put_object_fromdisk_callback)
	su_proc_decl(iflybox_put_object)
	su_proc_decl(iflybox_get_object_todisk_callback)
	su_proc_decl(iflybox_get_object)
	su_proc_decl(iflybox_remove_object)
	su_proc_decl(iflybox_copy_object)
	su_proc_decl(iflybox_move_object)
	su_proc_decl(iflybox_set_objectmeta)
	su_proc_decl(iflybox_get_objectmeta)
//	su_proc_decl(iflybox_remove_objectmeta)
	su_proc_decl(iflybox_objectmeta_is_pseudodirectory)
	su_proc_decl(iflybox_objectmeta_contentlength)
	su_proc_decl(iflybox_objectmeta_etag)
	su_proc_decl(iflybox_objectmeta_lastmodify)

	su_proc_decl(iflybox_multipart_upload_init)
	su_proc_decl(iflybox_multipart_upload_part)
	su_proc_decl(iflybox_multipart_upload_list_parts)
	su_proc_decl(iflybox_multipart_upload_complete)
	su_proc_decl(iflybox_multipart_upload_abort)

	su_proc_decl(iflybox_accountmeta_create)
	su_proc_decl(iflybox_accountmeta_release)
	su_proc_decl(iflybox_bucketmeta_create)
	su_proc_decl(iflybox_bucketmeta_release)
	su_proc_decl(iflybox_objectmeta_create)
	su_proc_decl(iflybox_objectmeta_release)

	su_proc_decl(iflybox_metadata_ownername)
	su_proc_decl(iflybox_metadata_get)
	su_proc_decl(iflybox_metadata_set)
	su_proc_decl(iflybox_metadata_count)
		
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
			ret = iflybox_lib_.open(libs.front().c_str());
			if(ret != 0)
			{
				dslog_error_assert(0, ("ibox_mngr::on_initiallize | can not load lib %ls.", libs.front().c_str()));
				return ret;
			}
			else
			{
				// 调用初始化系统接口
				// 读取iflybox相关配置项
				// wirte_localfile_
				sp::inisec isec_iflybox(cfg_mngr().cfg_file().c_str(), "iflybox");
				write_localfile_ = false;
				isec_iflybox.get_bool_value("write_localfile", write_localfile_);
               
				std::string endpoint, accessKeyId, accessKeySecret;
				int endpoint_type;
				//在dstester的配置中获取
				isec_iflybox.get_str_value("endpoint", endpoint);
				isec_iflybox.get_str_value("key", accessKeyId);
				isec_iflybox.get_str_value("password", accessKeySecret);
				isec_iflybox.get_int_value("endpoint_type", endpoint_type);
				
				
				dslog_perf_n(iflybox_create_instance);
		
				iflyboxResult result = iflybox_lib_.iflybox_init_result()();
				iflybox_handle_ = iflybox_lib_.iflybox_create_instance()(endpoint.c_str(), accessKeyId.c_str(), accessKeySecret.c_str(), endpoint_type, result);
				
				double nsec = dslog_perf_stop();
				proc_resp_mngr().add_nsec("iflybox_create_instance", nsec);
                
				if (iflybox_handle_ == NULL)
				{
					proc_resp_mngr().add_record("iflybox_create_instance_failed");
					dslog_error("ibox_mngr | iflybox_create_instance failed, result = %d, error msg:%s.", iflybox_lib_.iflybox_result_statuscode()(result), iflybox_lib_.iflybox_result_detail()(result));
					iflybox_lib_.iflybox_release_result()(result);
					return -1;
				}
				iflybox_lib_.iflybox_release_result()(result);
				dslog_crit("ibox_mngr | iflybox_create_instance success, time %4.3f.", nsec * 1e-9);
				proc_resp_mngr().add_record("iflybox_create_instance_success");


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


		dslog_perf_nsec(iflybox_release_instance, nsec);
		int ret = iflybox_lib_.iflybox_release_instance()(iflybox_handle_);
		proc_resp_mngr().add_nsec("iflybox_release_instance", dslog_perf_stop());
		
		if (ret)
		{
			proc_resp_mngr().add_record("iflybox_release_instance_failed");
			dslog_error("ibox_mngr | iflybox_release_instance failed, error = %d", ret);
			return error_code;
		}
		
		dslog_crit("ibox_mngr | iflybox_release_instance success, time %4.3f.", nsec * 1e-9);
		proc_resp_mngr().add_record("iflybox_release_instance_success");

		iflybox_lib_.close();

		return 0;
	}

public:
	ibox_mngr(void)
	{
	}

	iflybox_lib & iflybox(void)
	{
		return iflybox_lib_;
	}
	iBoxHandle iflybox_handle(void){
		return iflybox_handle_;
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
	iflybox_lib		iflybox_lib_;
	iBoxHandle		iflybox_handle_;
	bool write_localfile_;
};



class iflybox_svc
	: public sc::asc_cmd_t<iflybox_svc>
	, public svc_bind_unit<iflybox_unit>
{
#ifdef _WIN32
	typedef int (__thiscall iflybox_svc::*Proc_Metadata)(MetadataHandle);
#else
	typedef int (iflybox_svc::*Proc_Metadata)(MetadataHandle);
#endif
public:
	iflybox_svc(void);
	virtual ~iflybox_svc(void);

	virtual int on_begin();
	virtual int on_end();

protected:
	asc_cmd_entry_begin(iflybox_svc)
		asc_cmd_entry_pars(iflybox_svc, iflybox_reauth_account)
		asc_cmd_entry_pars(iflybox_svc, iflybox_bucket_exist)
		asc_cmd_entry_pars(iflybox_svc, iflybox_create_bucket)
		asc_cmd_entry_pars(iflybox_svc, iflybox_delete_bucket_ifempty)
		asc_cmd_entry_pars(iflybox_svc, iflybox_object_exist)
		asc_cmd_entry_pars(iflybox_svc, iflybox_put_object)
        asc_cmd_entry_pars(iflybox_svc, iflybox_get_object)
		asc_cmd_entry_pars(iflybox_svc, iflybox_remove_object)
		asc_cmd_entry_pars(iflybox_svc, iflybox_copy_object)
		asc_cmd_entry_pars(iflybox_svc, iflybox_move_object)
		asc_cmd_entry_pars(iflybox_svc, iflybox_list_buckets)
		asc_cmd_entry_pars(iflybox_svc, iflybox_list_object)
		asc_cmd_entry_pars(iflybox_svc, iflybox_accountmeta_create)
		asc_cmd_entry_pars(iflybox_svc, iflybox_bucketmeta_create)
		asc_cmd_entry_pars(iflybox_svc, iflybox_objectmeta_create)
		asc_cmd_entry_pars(iflybox_svc, iflybox_accountmeta_release)
		asc_cmd_entry_pars(iflybox_svc, iflybox_bucketmeta_release)
		asc_cmd_entry_pars(iflybox_svc, iflybox_objectmeta_release)
		asc_cmd_entry_pars(iflybox_svc, iflybox_get_objectmeta)
		asc_cmd_entry_pars(iflybox_svc, iflybox_get_bucketmeta)
		asc_cmd_entry_pars(iflybox_svc, iflybox_get_accountmeta)
		asc_cmd_entry_pars(iflybox_svc, iflybox_accountmeta_bucketcount)
		asc_cmd_entry_pars(iflybox_svc, iflybox_accountmeta_bytesused)
		asc_cmd_entry_pars(iflybox_svc, iflybox_bucketmeta_objectcount)
		asc_cmd_entry_pars(iflybox_svc, iflybox_bucketmeta_bytesused)
		asc_cmd_entry_pars(iflybox_svc, iflybox_objectmeta_contentlength)
		asc_cmd_entry_pars(iflybox_svc, iflybox_objectmeta_etag)
		asc_cmd_entry_pars(iflybox_svc, iflybox_objectmeta_lastmodify)
		asc_cmd_entry_pars(iflybox_svc, iflybox_multipart_upload_init)
		asc_cmd_entry_pars(iflybox_svc, iflybox_multipart_upload_part)
		asc_cmd_entry_pars(iflybox_svc, iflybox_multipart_upload_list_parts)
		asc_cmd_entry_pars(iflybox_svc, iflybox_multipart_upload_complete)
		asc_cmd_entry_pars(iflybox_svc, iflybox_multipart_upload_abort)
		asc_cmd_entry_pars(iflybox_svc, iflybox_metadata_set)
		asc_cmd_entry_pars(iflybox_svc, iflybox_set_bucketmeta)
		asc_cmd_entry_pars(iflybox_svc, iflybox_set_objectmeta)
		asc_cmd_entry_pars(iflybox_svc, iflybox_metadata_get)
		asc_cmd_entry_pars(iflybox_svc, iflybox_set_accountmeta)
//		asc_cmd_entry_pars(iflybox_svc, iflybox_remove_objectmeta)
		asc_cmd_entry_pars(iflybox_svc, iflybox_remove_bucketmeta)
		asc_cmd_entry_pars(iflybox_svc, iflybox_remove_accountmeta)
		asc_cmd_entry_pars(iflybox_svc, loop_metadata)
	asc_cmd_entry_end();
protected:
	virtual int on_iflybox_reauth_account(const sc::par_arr & pars);
	virtual int on_iflybox_bucket_exist(const sc::par_arr & pars);
	virtual int on_iflybox_create_bucket(const sc::par_arr & pars);
	virtual int on_iflybox_delete_bucket_ifempty(const sc::par_arr& pars);
	virtual int on_iflybox_object_exist(const sc::par_arr& pars);
	virtual int on_iflybox_put_object(const sc::par_arr& pars);
    virtual int on_iflybox_get_object(const sc::par_arr& pars);
	virtual int on_iflybox_remove_object(const sc::par_arr& pars);
	virtual int on_iflybox_copy_object(const sc::par_arr& pars);
	virtual int on_iflybox_move_object(const sc::par_arr& pars);
	virtual int on_iflybox_list_buckets(const sc::par_arr& pars);
	virtual int on_iflybox_list_object(const sc::par_arr& pars);
	virtual int on_iflybox_accountmeta_create(const sc::par_arr& pars);
	virtual int on_iflybox_bucketmeta_create(const sc::par_arr& pars);
	virtual int on_iflybox_objectmeta_create(const sc::par_arr& pars);
	virtual int on_iflybox_accountmeta_release(const sc::par_arr& pars);
	virtual int on_iflybox_bucketmeta_release(const sc::par_arr& pars);
	virtual int on_iflybox_objectmeta_release(const sc::par_arr& pars);
	virtual int on_iflybox_get_accountmeta(const sc::par_arr& pars);
	virtual int on_iflybox_get_bucketmeta(const sc::par_arr& pars);
	virtual int on_iflybox_get_objectmeta(const sc::par_arr& pars);
	virtual int on_iflybox_accountmeta_bucketcount(const sc::par_arr& pars);
	virtual int on_iflybox_accountmeta_bytesused(const sc::par_arr& pars);
	virtual int on_iflybox_bucketmeta_objectcount(const sc::par_arr& pars);
	virtual int on_iflybox_bucketmeta_bytesused(const sc::par_arr& pars);
	virtual int on_iflybox_objectmeta_contentlength(const sc::par_arr& pars);
	virtual int on_iflybox_objectmeta_etag(const sc::par_arr& pars);
	virtual int on_iflybox_objectmeta_lastmodify(const sc::par_arr& pars);
	virtual int on_iflybox_multipart_upload_init(const sc::par_arr& pars);
	virtual int on_iflybox_multipart_upload_part(const sc::par_arr& pars);
	virtual int on_iflybox_multipart_upload_list_parts(const sc::par_arr& pars);
	virtual int on_iflybox_multipart_upload_complete(const sc::par_arr& pars);
	virtual int on_iflybox_multipart_upload_abort(const sc::par_arr& pars);
	virtual int on_iflybox_metadata_set(const sc::par_arr& pars);
	virtual int on_iflybox_metadata_get(const sc::par_arr& pars);
	virtual int on_iflybox_set_accountmeta(const sc::par_arr& pars);
	virtual int on_iflybox_set_bucketmeta(const sc::par_arr& pars);
	virtual int on_iflybox_set_objectmeta(const sc::par_arr& pars);
//	virtual int on_iflybox_remove_objectmeta(const sc::par_arr& pars);
	virtual int on_iflybox_remove_bucketmeta(const sc::par_arr& pars);
	virtual int on_iflybox_remove_accountmeta(const sc::par_arr& pars);
	//自定义辅助工具
	virtual int on_loop_metadata(const sc::par_arr& pars);
	virtual int impl_iflybox_bucketmeta_objectcount(MetadataHandle handle);
	virtual int impl_iflybox_bucketmeta_bytesused(MetadataHandle handle);
	virtual int impl_iflybox_objectmeta_contentlength(MetadataHandle handle);
	virtual int impl_iflybox_objectmeta_etag(MetadataHandle handle);
	virtual int impl_iflybox_objectmeta_lastmodify(MetadataHandle handle);

	iflybox_lib & get_iflybox_lib(void);

private:
	iflybox_lib &	iflybox_lib_;

	iBoxHandle		iflybox_handle_;
	
	char* buf_value;

	bool				write_localfile_;
	BucketMetaHandle*	container_metas_;
	int					container_meta_cnt_;
	ObjectMetaHandle*	object_metas_;
	int					object_meta_cnt_;
	AccountMetaHandle	last_used_accountmeta_;
	BucketMetaHandle	last_used_bucketmeta_;
	ObjectMetaHandle	last_used_objectmeta_;
	UploadId			last_upload_id_;
	std::map<std::string, Proc_Metadata>	impl_procs;
};




typedef sc::asc_session_t<sc::asc_parser, svc_sink_impl<iflybox_svc>, char, 
	sc::asc_name_t<'i', 'f', 'l', 'y', 'b', 'o', 'x'> >
	iflybox_svc_parser;

typedef thr_admin_parser<ibox_mngr, iflybox_svc_parser, iflybox_unit> iflybox_mngr;

#endif	/* _IFLYBPX_SVC_H__ */
