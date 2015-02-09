#include "cssp_svc.h"
#include <errno.h>
#ifdef _WIN32
#include "iconv.h"
#endif

void AutoRecordUnitError(proc_resp<Log_Thread_Mutex, char>& mngr, const char* unit_name, int error, double perf){
	char stub[100];
#ifdef _WIN32
	sprintf_s(stub, 100, "%s-%d", unit_name, error);
#else
	snprintf(stub, 100, "%s-%d", unit_name, error);
#endif
	mngr.add_nsec(stub, perf);
}

#ifdef _WIN32
int code_convert(char *from_charset,char *to_charset,const char *inbuf, size_t inlen,char *outbuf, size_t outlen)
{
	iconv_t cd;
	const char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (!inbuf)
	{
		return -1;
	}
	if (cd == 0)
	{
		return -1;
	}
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
	{
		return -1;
	}
	iconv_close(cd);
	return 0;
}
#endif

cssp_svc::cssp_svc(void)
	: cssp_lib_(ibox_mngr::instance().cssp())
	, write_localfile_(ibox_mngr::instance().write_localfile())
	, cssp_handle_(ibox_mngr::instance().cssp_handle())
	,container_meta_cnt_(0),object_metas_(NULL)
	,object_meta_cnt_(0),last_used_objectmeta_(NULL){

	impl_procs["cssp_objectmeta_contentlength"] = &cssp_svc::impl_cssp_objectmeta_contentlength;
	impl_procs["cssp_objectmeta_etag"] = &cssp_svc::impl_cssp_objectmeta_etag;
	impl_procs["cssp_objectmeta_lastmodify"] = &cssp_svc::impl_cssp_objectmeta_lastmodify;
}

cssp_svc::~cssp_svc(void){
	
}

int cssp_svc::on_begin()
{	
	return 0;
}

int cssp_svc::on_end()
{
	return 0;
}

/*
int cssp_svc::on_cssp_delete_container_ifempty(const sc::par_arr & pars)
{
	dslog_tracer(cssp_svc::on_cssp_delete_container_ifempty);
	int status_code = 0;

	dslog_perf_n(cssp_delete_container_ifempty);
	CSSPResult result = cssp_lib_.cssp_init_result()();
	status_code = cssp_lib_.cssp_delete_container_ifempty()(cssp_handle_, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_delete_container_ifempty", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_delete_container_ifempty | cssp_delete_container_ifempty failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));
	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_delete_container_ifempty | cssp_delete_container_ifempty success.");
	AutoRecordUnitError(proc_resp_mngr(), "cssp_delete_container_ifempty", status_code, perf);
	return status_code;
}
*/

int cssp_svc::on_cssp_object_exist(const sc::par_arr & pars)
{
	dslog_tracer(cssp_svc::on_cssp_object_exist);

#ifdef _WIN32
	const char* objecttmp = pars.first_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* object = pars.first_par_str_value();
#endif

	int status_code = 0;

	dslog_perf_n(cssp_object_exist);
	CSSPResult result = cssp_lib_.cssp_init_result()();
	status_code = cssp_lib_.cssp_object_exist()(cssp_handle_, object, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_object_exist", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_object_exist | cssp_object_exist failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));
	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_object_exist | cssp_object_exist success.");
	AutoRecordUnitError(proc_resp_mngr(), "cssp_object_exist", status_code, perf);
	return status_code;
}


int cssp_svc::on_cssp_put_object(const sc::par_arr & pars)
{
	dslog_tracer(cssp_svc::on_cssp_put_object);

#ifdef _WIN32

	const char* objecttmp = pars.first_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* object = pars.first_par_str_value();
#endif
	const char* srcpath = pars.third_par_str_value();
	const char* md5 = pars.fourth_par_str_value();
	int status_code = 0;
	double perf = 0;
	
	CSSPResult result = cssp_lib_.cssp_init_result()();
	FILE* fd = fopen(srcpath, "rb");
	if(fd){
		dslog_perf_n(cssp_object_exist);
		status_code = cssp_lib_.cssp_put_object()(cssp_handle_, object, cssp_lib_.cssp_put_object_fromdisk_callback(), fd, 0, md5, NULL, result);
		perf = dslog_perf_stop();
		fclose(fd);
		proc_resp_mngr().add_nsec("cssp_put_object", perf);
	}else{
		status_code = -2;
	}
	

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_put_object | cssp_put_object failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));
	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_put_object | cssp_put_object success.");
	AutoRecordUnitError(proc_resp_mngr(), "cssp_put_object", status_code, perf);
	return status_code;
}


int cssp_svc::on_cssp_get_object(const sc::par_arr & pars)
{
	dslog_tracer(cssp_svc::on_cssp_get_object);

#ifdef _WIN32
	const char* objecttmp = pars.first_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* object = pars.first_par_str_value();
#endif

	const char* dstpath = pars.second_par_str_value();

	uint64_t offset = pars.third_par_int_value(0, 0);
	int64_t size = pars.fourth_par_int_value(0, 0);
	int status_code = 0;
	double perf = 0;
	cssp_get_object_callback_ptr callback_fun = NULL;
	void* outputstream = NULL;
	CSSPResult result = cssp_lib_.cssp_init_result()();
	if(write_localfile_){
		outputstream = fopen(dstpath, "wb");
	}
	else{
		outputstream = new char[size];
	}
	if(outputstream){
		dslog_perf_n(cssp_object_exist);
		if(write_localfile_)
			status_code = cssp_lib_.cssp_get_object()(cssp_handle_, object, cssp_lib_.cssp_get_object_todisk_callback(), outputstream, offset, &size, NULL, result);
		else
			status_code = cssp_lib_.cssp_get_object()(cssp_handle_, object, NULL, outputstream, offset, &size, NULL, result);
		perf = dslog_perf_stop();
		proc_resp_mngr().add_nsec("cssp_get_object", perf);
	}else{
		status_code = -2;
	}
	if(write_localfile_ && outputstream){
		fclose((FILE*)outputstream);
		if(status_code < 200 || status_code >= 300)
			remove(dstpath);
	}else{
		delete [] outputstream;
	}
	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_get_object | cssp_get_object failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));
	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_get_object | cssp_get_object success.");
	AutoRecordUnitError(proc_resp_mngr(), "cssp_get_object", status_code, perf);

	return status_code;
}

int cssp_svc::on_cssp_remove_object(const sc::par_arr & pars)
{
	dslog_tracer(cssp_svc::on_cssp_remove_object);

#ifdef _WIN32
	const char* objecttmp = pars.first_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* object = pars.first_par_str_value();
#endif

	int status_code = 0;

	dslog_perf_n(cssp_remove_object);
	CSSPResult result = cssp_lib_.cssp_init_result()();
	status_code = cssp_lib_.cssp_remove_object()(cssp_handle_, object, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_remove_object", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_remove_object | cssp_remove_object failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));
		
	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_remove_object | cssp_remove_object success.");
	AutoRecordUnitError(proc_resp_mngr(), "cssp_remove_object", status_code, perf);
	return status_code;
}

int cssp_svc::on_cssp_copy_object(const sc::par_arr & pars)
{
	dslog_tracer(cssp_svc::on_cssp_copy_object);

#ifdef _WIN32
	const char* srcobjecttmp = pars.first_par_str_value();
	char srcobject[1200];
	code_convert("GBK", "UTF-8", srcobjecttmp, strlen(srcobjecttmp), srcobject, 1200);

	const char* dstobjecttmp = pars.second_par_str_value();
	char dstobject[1200];
	code_convert("GBK", "UTF-8", dstobjecttmp, strlen(dstobjecttmp), dstobject, 1200);
#else;
	const char* srcobject = pars.first_par_str_value();
	const char* dstobject = pars.second_par_str_value();
#endif
	
	int status_code = 0;

	dslog_perf_n(cssp_copy_object);
	CSSPResult result = cssp_lib_.cssp_init_result()();
	status_code = cssp_lib_.cssp_copy_object()(cssp_handle_, srcobject, dstobject, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_copy_object", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_copy_object | cssp_copy_object failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));

	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_copy_object | cssp_copy_object success.");
	
	AutoRecordUnitError(proc_resp_mngr(), "cssp_copy_object", status_code, perf);
	return status_code;
}

int cssp_svc::on_cssp_move_object(const sc::par_arr & pars)
{
	dslog_tracer(cssp_svc::on_cssp_move_object);

#ifdef _WIN32
	const char* srcobjecttmp = pars.first_par_str_value();
	char srcobject[1200];
	code_convert("GBK", "UTF-8", srcobjecttmp, strlen(srcobjecttmp), srcobject, 1200);

	const char* dstobjecttmp = pars.second_par_str_value();
	char dstobject[1200];
	code_convert("GBK", "UTF-8", dstobjecttmp, strlen(dstobjecttmp), dstobject, 1200);
#else;
	const char* srcobject = pars.first_par_str_value();
	const char* dstobject = pars.second_par_str_value();
#endif

	int status_code = 0;

	dslog_perf_n(cssp_move_object);
	CSSPResult result = cssp_lib_.cssp_init_result()();
	status_code = cssp_lib_.cssp_move_object()(cssp_handle_, srcobject, dstobject, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_move_object", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_move_object | cssp_move_object failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));

	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_move_object | cssp_move_object success.");

	AutoRecordUnitError(proc_resp_mngr(), "cssp_move_object", status_code, perf);
	return status_code;
}


int cssp_svc::on_cssp_list_object(const sc::par_arr& pars){
	dslog_tracer(cssp_svc::on_cssp_list_object);

#ifdef _WIN32
	int limit = pars.first_par_int_value(0, 0);
	const char* prefixtmp = pars.second_par_str_value();
	char prefix[1200];
	memset(prefix, 0, 1200);
	if (prefixtmp)
	{
		code_convert("GBK", "UTF-8", prefixtmp, strlen(prefixtmp), prefix, 1200);
	}
	const char* delimitertmp = pars.third_par_str_value();
	char delimiter[1200];
	memset(delimiter, 0, 1200);
	if (delimitertmp)
	{
		code_convert("GBK", "UTF-8", delimitertmp, strlen(delimitertmp), delimiter, 1200);
	}
	const char* markertmp = pars.fourth_par_str_value();
	char marker[1200];
	memset(marker, 0, 1200);
	if (markertmp)
	{
		code_convert("GBK", "UTF-8", markertmp, strlen(markertmp), marker, 1200);
	}
#else
	int limit = pars.first_par_int_value(0, 0);
	const char* prefix = pars.second_par_str_value();
	const char* delimiter = pars.third_par_str_value();
	const char* marker = pars.fourth_par_str_value();
#endif
	int status_code = 0;
	const int MAX_LIMIT = 10000;


	dslog_perf_n(cssp_list_object);
	CSSPResult result = cssp_lib_.cssp_init_result()();
	object_meta_cnt_ = 0;
	do{
		const int list_wanted_cnt = (limit - object_meta_cnt_) > MAX_LIMIT ? MAX_LIMIT : (limit - object_meta_cnt_);
		int list_got_cnt = list_wanted_cnt;
		ObjectMetaHandle* tmp_metas = (list_got_cnt >= 0 ? new ObjectMetaHandle[list_got_cnt] : NULL);
		status_code = cssp_lib_.cssp_list_object()(cssp_handle_, tmp_metas, &list_got_cnt, prefix, delimiter, marker, result);
		ObjectMetaHandle* total_tmp = (list_got_cnt >= 0 ? new ObjectMetaHandle[object_meta_cnt_ + list_got_cnt] : NULL);
		if(object_meta_cnt_)
		{
			memcpy(total_tmp, object_metas_, sizeof(ObjectMetaHandle) * object_meta_cnt_);
		}
		if(list_got_cnt > 0){
#ifdef _WIN32
			const char *marker1 = cssp_lib_.cssp_metadata_ownername()(tmp_metas[list_got_cnt - 1]);
			strcpy(marker, marker1);
			memcpy(total_tmp + object_meta_cnt_, tmp_metas, sizeof(ObjectMetaHandle) * list_got_cnt);
#else
			marker = cssp_lib_.cssp_metadata_ownername()(tmp_metas[list_got_cnt - 1]);
			memcpy(total_tmp + object_meta_cnt_, tmp_metas, sizeof(ObjectMetaHandle) * list_got_cnt);
#endif
		}
		delete [] tmp_metas;
		delete [] object_metas_;
		object_metas_ = total_tmp;
		object_meta_cnt_ += list_got_cnt;
		if(list_got_cnt < list_wanted_cnt || list_got_cnt == 0)
			break;
	}while(status_code >= 200 && status_code < 300);

	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_list_object", perf);

	if (status_code < 200 || status_code >= 300 )
	{
		dslog_error("cssp_svc::on_cssp_list_object | cssp_list_object failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));

	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_list_object | cssp_list_object success.");

	AutoRecordUnitError(proc_resp_mngr(), "cssp_list_object", status_code, perf);
	return status_code;
}



int cssp_svc::on_cssp_objectmeta_create(const sc::par_arr& pars){
	last_used_objectmeta_ = cssp_lib_.cssp_objectmeta_create()();
	return 0;
}


int cssp_svc::on_cssp_objectmeta_release(const sc::par_arr& pars){
	cssp_lib_.cssp_objectmeta_release()(last_used_objectmeta_);
	return 0;
}


int cssp_svc::on_cssp_get_objectmeta(const sc::par_arr& pars){
	dslog_tracer(cssp_svc::on_cssp_get_objectmeta);
	if(last_used_objectmeta_ == NULL){
		dslog_error("cssp_svc::on_cssp_get_objectmeta | ObjectMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;
#ifdef _WIN32
	const char* objecttmp = pars.first_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* object = pars.first_par_str_value();
#endif
	dslog_perf_n(cssp_get_objectmeta);
	CSSPResult result = cssp_lib_.cssp_init_result()();
	status_code = cssp_lib_.cssp_get_objectmeta()(cssp_handle_, object, last_used_objectmeta_, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_get_objectmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_get_objectmeta | cssp_get_objectmeta failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));
	}
	else{
		dslog_info("cssp_svc::on_cssp_get_containermeta | object %s", object);
	}
	cssp_lib_.cssp_release_result()(result);
	
	AutoRecordUnitError(proc_resp_mngr(), "cssp_get_objectmeta", status_code, perf);
	return status_code;
}

int cssp_svc::on_cssp_set_objectmeta(const sc::par_arr& pars){
	dslog_tracer(cssp_svc::on_cssp_set_objectmeta);
	if(last_used_objectmeta_ == NULL){
		dslog_error("cssp_svc::on_cssp_set_objectmeta | ObjectMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;
#ifdef _WIN32
	const char* objecttmp = pars.first_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* first = pars.second_par_str_value();
#endif
	
	dslog_perf_n(cssp_set_objectmeta);
	CSSPResult result = cssp_lib_.cssp_init_result()();
	status_code = cssp_lib_.cssp_set_objectmeta()(cssp_handle_, object, last_used_objectmeta_, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_set_objectmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_set_objectmeta | cssp_set_objectmeta failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));
	}
	else{
		dslog_info("cssp_svc::on_cssp_set_objectmeta | object %s", object);
	}
	cssp_lib_.cssp_release_result()(result);

	AutoRecordUnitError(proc_resp_mngr(), "cssp_set_objectmeta", status_code, perf);
	return status_code;
}


int cssp_svc::on_cssp_metadata_set(const sc::par_arr& pars){
	dslog_tracer(cssp_svc::on_cssp_metadata_set);

	int status_code = 0;
	const char* metatype = pars.first_par_str_value();
	MetadataHandle metas_hdl = NULL;
	if(metatype){	
		if(strcmp(metatype, "object") == 0)
		{
			metas_hdl = last_used_objectmeta_;
		}
	}
	if(metas_hdl == NULL){
		dslog_error("cssp_svc::on_cssp_metadata_set | metatype error!");
		return -1;
	}
#ifdef _WIN32
	const char* metanametmp = pars.second_par_str_value();
	char metaname[1200]; 
	code_convert("GBK", "UTF-8", metanametmp, strlen(metanametmp), metaname, 1200);

	const char* metavaluetmp = pars.third_par_str_value();
	char metavalue[1200]; 
	code_convert("GBK", "UTF-8", metavaluetmp, strlen(metavaluetmp), metavalue, 1200);
#else
	const char* metaname = pars.second_par_str_value();

	const char* metavalue = pars.third_par_str_value();
#endif
	


	dslog_perf_n(cssp_metadata_set);
//	CSSPResult result = cssp_lib_.cssp_init_result()();
	status_code = cssp_lib_.cssp_metadata_set()(metas_hdl, metaname, metavalue);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_metadata_set", perf);

	if (status_code)
	{
		dslog_error("cssp_svc::on_cssp_metadata_set | cssp_metadata_set failed. status_code = %d", status_code);
	}
	else{
		dslog_info("cssp_svc::on_cssp_metadata_set | metaname/metavalue %s/%s", metaname, metavalue);
	}
//	cssp_lib_.cssp_release_result()(result);

	AutoRecordUnitError(proc_resp_mngr(), "cssp_metadata_set", status_code, perf);
	return status_code;
}

int cssp_svc::on_cssp_metadata_get(const sc::par_arr& pars){
	dslog_tracer(cssp_svc::on_cssp_metadata_get);

	int status_code = 0;
	const char* metatype = pars.first_par_str_value();
	MetadataHandle metas_hdl = NULL;
	if(metatype){
		if(strcmp(metatype, "object") == 0)
		{
			metas_hdl = last_used_objectmeta_;
		}
	}
	if(metas_hdl == NULL){
		dslog_error("cssp_svc::on_cssp_metadata_get | metatype error!");
		return -1;
	}
#ifdef _WIN32
	const char* metanametmp = pars.second_par_str_value();
	char metaname[1200]; 
	code_convert("GBK", "UTF-8", metanametmp, strlen(metanametmp), metaname, 1200);
#else
	const char* metaname = pars.second_par_str_value();
#endif
	dslog_perf_n(cssp_metadata_get);
	//	CSSPResult result = cssp_lib_.cssp_init_result()();
	const char* metavalue = cssp_lib_.cssp_metadata_get()(metas_hdl, metaname);
	if (metavalue)
		status_code = 0;
	else
		status_code = -1;
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_metadata_get", perf);

	if (status_code)
	{
		dslog_error("cssp_svc::on_cssp_metadata_get | cssp_metadata_get failed. status_code = %d", status_code);
	}
	else{
		dslog_info("cssp_svc::on_cssp_metadata_get | metaname/metavalue %s/%s", metaname, metavalue);
	}
	//	cssp_lib_.cssp_release_result()(result);

	AutoRecordUnitError(proc_resp_mngr(), "cssp_metadata_get", status_code, perf);
	return status_code;
}


int cssp_svc::on_cssp_count_object(const sc::par_arr& pars){
	return impl_cssp_count_object();
}

int cssp_svc::impl_cssp_count_object(){
	CSSPResult result = cssp_lib_.cssp_init_result()();
	uint64_t object_count = 0;
	int status_code = cssp_lib_.cssp_count_object()(cssp_handle_, &object_count, result);
#ifdef _WIN32
	dslog_info("cssp_svc::on_cssp_containermeta_objectcount | objectCount = %I64u.", object_count);
#elif _M_X64
	dslog_info("cssp_svc::on_cssp_containermeta_objectcount | objectCount = %lu.", object_count);
#elif _M_IX86
	dslog_info("cssp_svc::on_cssp_containermeta_objectcount | objectCount = %llu.", object_count);
#else
#error "Not found _WIN32 or _M_X64 or _M_IX86!"
#endif
	cssp_lib_.cssp_release_result()(result);
	return status_code;
}

int cssp_svc::on_cssp_count_bytesused(const sc::par_arr& pars){
	return impl_cssp_count_bytesused();
}

int cssp_svc::impl_cssp_count_bytesused(){
	CSSPResult result = cssp_lib_.cssp_init_result()();
	uint64_t bytesused = 0;
	int status_code = cssp_lib_.cssp_count_bytesused()(cssp_handle_, &bytesused, result);
#ifdef _WIN32
	dslog_info("cssp_svc::on_cssp_containermeta_bytesused | bytesused = %I64u.", bytesused);
#elif _M_X64
	dslog_info("cssp_svc::on_cssp_containermeta_bytesused | bytesused = %lu.", bytesused);
#elif _M_IX86
	dslog_info("cssp_svc::on_cssp_containermeta_bytesused | bytesused = %llu.", bytesused);
#else
#error "Not found _WIN32 or _M_X64 or _M_IX86!"
#endif
	return status_code;
}

int cssp_svc::on_cssp_objectmeta_contentlength(const sc::par_arr& pars){
	return impl_cssp_objectmeta_contentlength(last_used_objectmeta_);
}


int cssp_svc::impl_cssp_objectmeta_contentlength(MetadataHandle handle){
	if(handle == NULL){
		dslog_error("cssp_svc::on_cssp_objectmeta_contentlength | ObjectMetaHandle Invalid!");
		return -1;
	}
	uint64_t congtentlength = cssp_lib_.cssp_objectmeta_contentlength()(handle);
#ifdef _WIN32
	dslog_info("cssp_svc::on_cssp_objectmeta_contentlength | name = %s, congtentlength = %I64u.", cssp_lib_.cssp_metadata_ownername()(handle), congtentlength);
#elif _M_X64
	dslog_info("cssp_svc::on_cssp_objectmeta_contentlength | name = %s, congtentlength = %lu.", cssp_lib_.cssp_metadata_ownername()(handle), congtentlength);
#elif _M_IX86
	dslog_info("cssp_svc::on_cssp_objectmeta_contentlength | name = %s, congtentlength = %llu.", cssp_lib_.cssp_metadata_ownername()(handle), congtentlength);
#else
#error "Not found _WIN32 or _M_X64 or _M_IX86!"
#endif
	return 0;
}

int cssp_svc::on_cssp_objectmeta_etag(const sc::par_arr& pars){
	return impl_cssp_objectmeta_etag(last_used_objectmeta_);
}

int cssp_svc::impl_cssp_objectmeta_etag(MetadataHandle handle){
	if(handle == NULL){
		dslog_error("cssp_svc::on_cssp_objectmeta_etag | ObjectMetaHandle Invalid!");
		return -1;
	}
	const char* etag = cssp_lib_.cssp_objectmeta_etag()(handle);
	dslog_info("cssp_svc::on_cssp_objectmeta_etag | name = %s, etag = %s.", cssp_lib_.cssp_metadata_ownername()(handle), etag);
	return 0;
}

int cssp_svc::on_cssp_objectmeta_lastmodify(const sc::par_arr& pars){
	return impl_cssp_objectmeta_lastmodify(last_used_objectmeta_);
}

int cssp_svc::impl_cssp_objectmeta_lastmodify(MetadataHandle handle){
	if(handle == NULL){
		dslog_error("cssp_svc::on_cssp_objectmeta_lastmodify | ObjectMetaHandle Invalid!");
		return -1;
	}
	const char* lastmodify = cssp_lib_.cssp_objectmeta_lastmodify()(handle);
	dslog_info("cssp_svc::on_cssp_objectmeta_lastmodify | name = %s, lastmodify = %s.", cssp_lib_.cssp_metadata_ownername()(handle), lastmodify);
	return 0;
}





int cssp_svc::on_loop_metadata(const sc::par_arr& pars){
	const char* metatype = pars.first_par_str_value();
	MetadataHandle* loop_metas = NULL;
	int loop_size = 0;
	if(metatype){
		if(strcmp(metatype, "object") == 0){
			loop_metas = object_metas_;
			loop_size = object_meta_cnt_;
		}
	}
	if(loop_metas == NULL){
		dslog_error("cssp_svc::on_loop_metadata | metatype error!");
		return -1;
	}
	
	//handle loop
	for(int index = 0; index < loop_size; ++index)
	{
		//proc loop
		for(int proc_idx = 1; proc_idx < pars.size(); ++proc_idx){
			std::string proc_name = pars.par_str_value(proc_idx, NULL, NULL);
			Proc_Metadata proc_point = impl_procs[proc_name];
			if(proc_point == NULL){
				dslog_error("cssp_svc::on_loop_metadata | proc %s not exist!", proc_name.c_str());
				continue;
			}
			
			(this->*proc_point)(loop_metas[index]);
			
		}
	}
	dslog_info("cssp_svc::on_loop_metadata | loop count = %d.", loop_size);
	return 0;
}


int cssp_svc::on_cssp_multipart_upload_init(const sc::par_arr& pars){
	dslog_tracer(cssp_svc::on_cssp_multipart_upload_init);
#ifdef _WIN32
	const char* objecttmp = pars.first_par_str_value();
	char object[1200]; 
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* object = pars.first_par_str_value();
#endif

	int status_code = 0;

	dslog_perf_n(cssp_multipart_upload_init);
	
	status_code = cssp_lib_.cssp_multipart_upload_init()(cssp_handle_, object, last_upload_id_);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_multipart_upload_init", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_multipart_upload_init | cssp_multipart_upload_init failed. status_code = %d.", status_code);
	}
	dslog_verbose("cssp_svc::on_cssp_multipart_upload_init | cssp_multipart_upload_init success.");
	dslog_info("cssp_svc::on_cssp_multipart_upload_init | %s UploadId = %s.", object, last_upload_id_);
	AutoRecordUnitError(proc_resp_mngr(), "cssp_multipart_upload_init", status_code, perf);
	return status_code;
}

struct multipart_input
{
	FILE *ffd;
	int offset;
	uint64_t length;
};

size_t multipart_callback(void* buffer, size_t size, size_t nmemb, void* inputstream)
{
	FILE* fd = reinterpret_cast<multipart_input *>(inputstream)->ffd;
	uint64_t l = reinterpret_cast<multipart_input *>(inputstream)->length;
	int ofs = reinterpret_cast<multipart_input *>(inputstream)->offset;
	if (ofs + nmemb * size >= l)
	{
		uint64_t read_size = fread(buffer, size, (l - ofs) / size, fd) * size;
		reinterpret_cast<multipart_input *>(inputstream)->offset = ofs + read_size;
		return read_size;
	}
	reinterpret_cast<multipart_input *>(inputstream)->offset = ofs + size * nmemb;
	return fread(buffer, size, nmemb, fd) * size;
}

int cssp_svc::on_cssp_multipart_upload_part(const sc::par_arr& pars){
	dslog_tracer(cssp_svc::on_cssp_multipart_upload_part);
	int partnum = pars.first_par_int_value(0, 0);
	const char* srcpath = pars.second_par_str_value();
	int offset = pars.third_par_int_value(0, 0);
	uint64_t length = pars.fourth_par_int_value(0, 0);
	const char* md5 = pars.fifth_par_str_value();
	int status_code = 0;
	double perf = 0;

	CSSPResult result = cssp_lib_.cssp_init_result()();
	FILE* fd = fopen(srcpath, "rb");
	if(fd){
/*		char* buf = new char[length];
		if(buf == NULL)
			return -1;*/
		fseek(fd, offset, SEEK_SET);
		multipart_input minput;
		minput.ffd = fd;
		minput.length = length;
		minput.offset = offset;
		dslog_perf_n(cssp_object_exist);
		status_code = cssp_lib_.cssp_multipart_upload_part()(cssp_handle_, last_upload_id_, partnum, multipart_callback/*cssp_lib_.cssp_put_object_fromdisk_callback()*/, &minput, 0, md5, result);
		perf = dslog_perf_stop();
		fclose(fd);
		proc_resp_mngr().add_nsec("cssp_multipart_upload_part", perf);
	}else{
		status_code = -2;
	}


	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_multipart_upload_part | cssp_multipart_upload_part failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));
	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_multipart_upload_part | cssp_multipart_upload_part success.");
	AutoRecordUnitError(proc_resp_mngr(), "cssp_multipart_upload_part", status_code, perf);
	return status_code;
}

int cssp_svc::on_cssp_multipart_upload_list_parts(const sc::par_arr& pars){
	dslog_tracer(cssp_svc::on_cssp_multipart_upload_list_parts);

	
	int limit = pars.first_par_int_value(0, 0);
#ifdef _WIN32
	const char* markertmp = pars.second_par_str_value();
	char marker[1200];
	memset(marker, 0, 1200);
	if (markertmp)
	{
		code_convert("GBK", "UTF-8", markertmp, strlen(markertmp), marker, 1200);
	}
#else
	const char* marker = pars.second_par_str_value();
#endif
	int status_code = 0;
	const int MAX_LIMIT = 10000;


	dslog_perf_n(cssp_multipart_upload_list_parts);
	CSSPResult result = cssp_lib_.cssp_init_result()();
	object_meta_cnt_ = 0;
	do{
		const int list_wanted_cnt = (limit - object_meta_cnt_) > MAX_LIMIT ? MAX_LIMIT : (limit - object_meta_cnt_);
		int list_got_cnt = list_wanted_cnt;
		ObjectMetaHandle* tmp_metas = (list_got_cnt >= 0) ? new ObjectMetaHandle[list_got_cnt] : NULL;
		status_code = cssp_lib_.cssp_multipart_upload_list_parts()(cssp_handle_, last_upload_id_, tmp_metas, &list_got_cnt, marker, result);
		ObjectMetaHandle* total_tmp = (list_got_cnt >= 0) ? new ObjectMetaHandle[object_meta_cnt_ + list_got_cnt] : NULL;
		if(object_meta_cnt_)
		{
			memcpy(total_tmp, object_metas_, sizeof(ObjectMetaHandle) * object_meta_cnt_);
		}
		if(list_got_cnt > 0){
#ifdef _WIN32
			const char *marker1 = cssp_lib_.cssp_metadata_ownername()(tmp_metas[list_got_cnt - 1]);
			strcpy(marker, marker1);
#else
            marker = cssp_lib_.cssp_metadata_ownername()(tmp_metas[list_got_cnt - 1]);
#endif
			memcpy(total_tmp + object_meta_cnt_, tmp_metas, sizeof(ObjectMetaHandle) * list_got_cnt);
		}
		delete [] tmp_metas;
		delete [] object_metas_;
		object_metas_ = total_tmp;
		object_meta_cnt_ += list_got_cnt;
		if(list_got_cnt < list_wanted_cnt || list_got_cnt == 0)
			break;
	}while(status_code >= 200 && status_code < 300);

	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_multipart_upload_list_parts", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_multipart_upload_list_parts | cssp_multipart_upload_list_parts failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));

	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_multipart_upload_list_parts | cssp_multipart_upload_list_parts success.");

	AutoRecordUnitError(proc_resp_mngr(), "cssp_multipart_upload_list_parts", status_code, perf);
	return status_code;
}

int cssp_svc::on_cssp_multipart_upload_complete(const sc::par_arr& pars){
	dslog_tracer(cssp_svc::on_cssp_multipart_upload_complete);
	int status_code = 0;
	double perf = 0;

	CSSPResult result = cssp_lib_.cssp_init_result()();
	
	dslog_perf_n(cssp_object_exist);
	status_code = cssp_lib_.cssp_multipart_upload_complete()(cssp_handle_, last_upload_id_, result);
	perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_multipart_upload_complete", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_multipart_upload_complete | cssp_multipart_upload_complete failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));
	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_multipart_upload_complete | cssp_multipart_upload_complete success.");
	AutoRecordUnitError(proc_resp_mngr(), "cssp_multipart_upload_complete", status_code, perf);
	return status_code;
}

int cssp_svc::on_cssp_multipart_upload_abort(const sc::par_arr& pars){
	dslog_tracer(cssp_svc::on_cssp_multipart_upload_abort);
	int status_code = 0;
	double perf = 0;

	CSSPResult result = cssp_lib_.cssp_init_result()();

	dslog_perf_n(cssp_object_exist);
	status_code = cssp_lib_.cssp_multipart_upload_abort()(cssp_handle_, last_upload_id_, result);
	perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("cssp_multipart_upload_abort", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("cssp_svc::on_cssp_multipart_upload_abort | cssp_multipart_upload_abort failed. status_code = %d, error message:%s", status_code, cssp_lib_.cssp_result_detail()(result));
	}
	cssp_lib_.cssp_release_result()(result);
	dslog_verbose("cssp_svc::on_cssp_multipart_upload_abort | cssp_multipart_upload_abort success.");
	AutoRecordUnitError(proc_resp_mngr(), "cssp_multipart_upload_abort", status_code, perf);
	return status_code;
}
