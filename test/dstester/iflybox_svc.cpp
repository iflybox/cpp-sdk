#include "iflybox_svc.h"
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

iflybox_svc::iflybox_svc(void)
	: iflybox_lib_(ibox_mngr::instance().iflybox())
	, write_localfile_(ibox_mngr::instance().write_localfile())
	, iflybox_handle_(ibox_mngr::instance().iflybox_handle())
	,container_metas_(NULL) ,container_meta_cnt_(0)
	,object_metas_(NULL), object_meta_cnt_(0)
	,last_used_accountmeta_(NULL) 
	,last_used_bucketmeta_(NULL)
	,last_used_objectmeta_(NULL){
	impl_procs["iflybox_bucketmeta_objectcount"] = &iflybox_svc::impl_iflybox_bucketmeta_objectcount;
	impl_procs["iflybox_bucketmeta_bytesused"] = &iflybox_svc::impl_iflybox_bucketmeta_bytesused;
	impl_procs["iflybox_objectmeta_contentlength"] = &iflybox_svc::impl_iflybox_objectmeta_contentlength;
	impl_procs["iflybox_objectmeta_etag"] = &iflybox_svc::impl_iflybox_objectmeta_etag;
	impl_procs["iflybox_objectmeta_lastmodify"] = &iflybox_svc::impl_iflybox_objectmeta_lastmodify;
}

iflybox_svc::~iflybox_svc(void){
	
}

int iflybox_svc::on_begin()
{	
	return 0;
}

int iflybox_svc::on_end()
{
	return 0;
}


int iflybox_svc::on_iflybox_bucket_exist(const sc::par_arr & pars)
{
	dslog_tracer(iflybox_svc::on_iflybox_bucket_exist);

#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
#else
	const char* container = pars.first_par_str_value();
#endif

	int status_code = 0;

	dslog_perf_n(iflybox_bucket_exists);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_bucket_exist()(iflybox_handle_, container, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_bucket_exist", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_bucket_exist | iflybox_bucket_exist failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	
	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_bucket_exist | iflybox_bucket_exist success.");
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_bucket_exist", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_create_bucket(const sc::par_arr & pars)
{
	dslog_tracer(iflybox_svc::on_iflybox_create_bucket);

#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
#else
	const char* container = pars.first_par_str_value();
#endif

	int status_code = 0;

	dslog_perf_n(iflybox_create_bucket);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_create_bucket()(iflybox_handle_, container, NULL, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_create_bucket", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_create_bucket | iflybox_create_bucket failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_create_bucket | iflybox_create_bucket success.");
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_create_bucket", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_delete_bucket_ifempty(const sc::par_arr & pars)
{
	dslog_tracer(iflybox_svc::on_iflybox_delete_bucket_ifempty);

#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
#else
	const char* container = pars.first_par_str_value();
#endif

	int status_code = 0;

	dslog_perf_n(iflybox_delete_bucket_ifempty);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_delete_bucket_ifempty()(iflybox_handle_, container, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_delete_bucket_ifempty", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_delete_bucket_ifempty | iflybox_delete_bucket_ifempty failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_delete_bucket_ifempty | iflybox_delete_bucket_ifempty success.");
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_delete_bucket_ifempty", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_object_exist(const sc::par_arr & pars)
{
	dslog_tracer(iflybox_svc::on_iflybox_object_exist);

#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);

	const char* objecttmp = pars.second_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* container = pars.first_par_str_value();
	const char* object = pars.second_par_str_value();
#endif

	int status_code = 0;

	dslog_perf_n(iflybox_object_exist);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_object_exist()(iflybox_handle_, container, object, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_object_exist", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_object_exist | iflybox_object_exist failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_object_exist | iflybox_object_exist success.");
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_object_exist", status_code, perf);
	return status_code;
}


int iflybox_svc::on_iflybox_put_object(const sc::par_arr & pars)
{
	dslog_tracer(iflybox_svc::on_iflybox_put_object);

#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);

	const char* objecttmp = pars.second_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* container = pars.first_par_str_value();
	const char* object = pars.second_par_str_value();
#endif
	const char* srcpath = pars.third_par_str_value();
	const char* md5 = pars.fourth_par_str_value();
	int status_code = 0;
	double perf = 0;
	
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	FILE* fd = fopen(srcpath, "rb");
	if(fd){
		dslog_perf_n(iflybox_object_exist);
		status_code = iflybox_lib_.iflybox_put_object()(iflybox_handle_, container, object, iflybox_lib_.iflybox_put_object_fromdisk_callback(), fd, 0, md5, NULL, result);
		perf = dslog_perf_stop();
		fclose(fd);
		proc_resp_mngr().add_nsec("iflybox_put_object", perf);
	}else{
		status_code = -2;
	}
	

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_put_object | iflybox_put_object failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_put_object | iflybox_put_object success.");
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_put_object", status_code, perf);
	return status_code;
}


int iflybox_svc::on_iflybox_get_object(const sc::par_arr & pars)
{
	dslog_tracer(iflybox_svc::on_iflybox_get_object);

#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);

	const char* objecttmp = pars.second_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* container = pars.first_par_str_value();
	const char* object = pars.second_par_str_value();
#endif

	const char* dstpath = pars.third_par_str_value();

	uint64_t offset = pars.fourth_par_int_value(0, 0);
	int64_t size = pars.fifth_par_int_value(0, 0);
	int status_code = 0;
	double perf = 0;
	iflybox_get_object_callback_ptr callback_fun = NULL;
	void* outputstream = NULL;
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	if(write_localfile_){
		outputstream = fopen(dstpath, "wb");
	}
	else{
		outputstream = new char[size];
	}
	if(outputstream){
		dslog_perf_n(iflybox_object_exist);
		if(write_localfile_)
			status_code = iflybox_lib_.iflybox_get_object()(iflybox_handle_, container, object, iflybox_lib_.iflybox_get_object_todisk_callback(), outputstream, offset, &size, NULL, result);
		else
			status_code = iflybox_lib_.iflybox_get_object()(iflybox_handle_, container, object, NULL, outputstream, offset, &size, NULL, result);
		perf = dslog_perf_stop();
		proc_resp_mngr().add_nsec("iflybox_get_object", perf);
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
		dslog_error("iflybox_svc::on_iflybox_get_object | iflybox_get_object failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_get_object | iflybox_get_object success.");
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_get_object", status_code, perf);

	return status_code;
}

int iflybox_svc::on_iflybox_remove_object(const sc::par_arr & pars)
{
	dslog_tracer(iflybox_svc::on_iflybox_remove_object);

#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
	const char* objecttmp = pars.second_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* container = pars.first_par_str_value();
	const char* object = pars.second_par_str_value();
#endif

	int status_code = 0;

	dslog_perf_n(iflybox_remove_object);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_remove_object()(iflybox_handle_, container, object, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_remove_object", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_remove_object | iflybox_remove_object failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
		
	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_remove_object | iflybox_remove_object success.");
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_remove_object", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_copy_object(const sc::par_arr & pars)
{
	dslog_tracer(iflybox_svc::on_iflybox_copy_object);

#ifdef _WIN32
	const char* srccontainertmp = pars.first_par_str_value();
	char srccontainer[1200];
	code_convert("GBK", "UTF-8", srccontainertmp, strlen(srccontainertmp), srccontainer, 1200);
	const char* srcobjecttmp = pars.second_par_str_value();
	char srcobject[1200];
	code_convert("GBK", "UTF-8", srcobjecttmp, strlen(srcobjecttmp), srcobject, 1200);
	const char* dstcontainertmp = pars.third_par_str_value();
	char dstcontainer[1200];
	code_convert("GBK", "UTF-8", dstcontainertmp, strlen(dstcontainertmp), dstcontainer, 1200);
	const char* dstobjecttmp = pars.fourth_par_str_value();
	char dstobject[1200];
	code_convert("GBK", "UTF-8", dstobjecttmp, strlen(dstobjecttmp), dstobject, 1200);
#else
	const char* srccontainer = pars.first_par_str_value();
	const char* srcobject = pars.second_par_str_value();
	const char* dstcontainer = pars.third_par_str_value();
	const char* dstobject = pars.fourth_par_str_value();
#endif
	
	int status_code = 0;

	dslog_perf_n(iflybox_copy_object);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_copy_object()(iflybox_handle_, srccontainer, srcobject, dstcontainer, dstobject, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_copy_object", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_copy_object | iflybox_copy_object failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));

	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_copy_object | iflybox_copy_object success.");
	
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_copy_object", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_move_object(const sc::par_arr & pars)
{
	dslog_tracer(iflybox_svc::on_iflybox_move_object);

#ifdef _WIN32
	const char* srccontainertmp = pars.first_par_str_value();
	char srccontainer[1200];
	code_convert("GBK", "UTF-8", srccontainertmp, strlen(srccontainertmp), srccontainer, 1200);
	const char* srcobjecttmp = pars.second_par_str_value();
	char srcobject[1200];
	code_convert("GBK", "UTF-8", srcobjecttmp, strlen(srcobjecttmp), srcobject, 1200);
	const char* dstcontainertmp = pars.third_par_str_value();
	char dstcontainer[1200];
	code_convert("GBK", "UTF-8", dstcontainertmp, strlen(dstcontainertmp), dstcontainer, 1200);
	const char* dstobjecttmp = pars.fourth_par_str_value();
	char dstobject[1200];
	code_convert("GBK", "UTF-8", dstobjecttmp, strlen(dstobjecttmp), dstobject, 1200);
#else
	const char* srccontainer = pars.first_par_str_value();
	const char* srcobject = pars.second_par_str_value();
	const char* dstcontainer = pars.third_par_str_value();
	const char* dstobject = pars.fourth_par_str_value();
#endif
	int status_code = 0;

	dslog_perf_n(iflybox_move_object);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_move_object()(iflybox_handle_, srccontainer, srcobject, dstcontainer, dstobject, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_move_object", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_move_object | iflybox_move_object failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));

	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_move_object | iflybox_move_object success.");

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_move_object", status_code, perf);
	return status_code;
}


int iflybox_svc::on_iflybox_list_buckets(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_list_buckets);

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


	dslog_perf_n(iflybox_list_buckets);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	container_meta_cnt_ = 0;
	do{
		const int list_wanted_cnt = (limit - container_meta_cnt_) > MAX_LIMIT ? MAX_LIMIT : (limit - container_meta_cnt_);
		int list_got_cnt = list_wanted_cnt;
		BucketMetaHandle* tmp_metas = (list_got_cnt >= 0) ? new BucketMetaHandle[list_got_cnt] : NULL;
		status_code = iflybox_lib_.iflybox_list_buckets()(iflybox_handle_, tmp_metas, &list_got_cnt, marker, result);
		BucketMetaHandle* total_tmp = (list_got_cnt >= 0) ? new BucketMetaHandle[container_meta_cnt_ + list_got_cnt] : NULL;
		if(container_meta_cnt_)
		{
			memcpy(total_tmp, container_metas_, sizeof(BucketMetaHandle) * container_meta_cnt_);
		}
		if(list_got_cnt > 0){
#ifdef _WIN32
			const char *marker1 = iflybox_lib_.iflybox_metadata_ownername()(tmp_metas[list_got_cnt - 1]);
			strcpy(marker,marker1);
#else
			marker = iflybox_lib_.iflybox_metadata_ownername()(tmp_metas[list_got_cnt - 1]);
#endif
			memcpy(total_tmp + container_meta_cnt_, tmp_metas, sizeof(BucketMetaHandle) * list_got_cnt);
		}
		delete [] tmp_metas;
		delete [] container_metas_;
		container_metas_ = total_tmp;
		container_meta_cnt_ += list_got_cnt;
	
		if(list_got_cnt < list_wanted_cnt || list_got_cnt == 0)
			break;
	}while(status_code >= 200 && status_code < 300);

	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_list_buckets", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_list_buckets | iflybox_list_buckets failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));

	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_list_buckets | iflybox_list_buckets success.");

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_list_buckets", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_list_object(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_list_object);

#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
	int limit = pars.second_par_int_value(0, 0);
	const char* prefixtmp = pars.third_par_str_value();
	char prefix[1200];
	memset(prefix, 0, 1200);
	if (prefixtmp)
	{
		code_convert("GBK", "UTF-8", prefixtmp, strlen(prefixtmp), prefix, 1200);
	}
	const char* delimitertmp = pars.fourth_par_str_value();
	char delimiter[1200];
	memset(delimiter, 0, 1200);
	if (delimitertmp)
	{
		code_convert("GBK", "UTF-8", delimitertmp, strlen(delimitertmp), delimiter, 1200);
	}
	const char* markertmp = pars.fifth_par_str_value();
	char marker[1200];
	memset(marker, 0, 1200);
	if (markertmp)
	{
		code_convert("GBK", "UTF-8", markertmp, strlen(markertmp), marker, 1200);
	}
#else
	const char* container = pars.first_par_str_value();
	int limit = pars.second_par_int_value(0, 0);
	const char* prefix = pars.third_par_str_value();
	const char* delimiter = pars.fourth_par_str_value();
	const char* marker = pars.fifth_par_str_value();
#endif
	int status_code = 0;
	const int MAX_LIMIT = 10000;


	dslog_perf_n(iflybox_list_object);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	object_meta_cnt_ = 0;
	do{
		const int list_wanted_cnt = (limit - object_meta_cnt_) > MAX_LIMIT ? MAX_LIMIT : (limit - object_meta_cnt_);
		int list_got_cnt = list_wanted_cnt;
		ObjectMetaHandle* tmp_metas = (list_got_cnt >= 0 ? new ObjectMetaHandle[list_got_cnt] : NULL);
		status_code = iflybox_lib_.iflybox_list_object()(iflybox_handle_, container, tmp_metas, &list_got_cnt, prefix, delimiter, marker, result);
		ObjectMetaHandle* total_tmp = (list_got_cnt >= 0 ? new ObjectMetaHandle[object_meta_cnt_ + list_got_cnt] : NULL);
		if(object_meta_cnt_)
		{
			memcpy(total_tmp, object_metas_, sizeof(ObjectMetaHandle) * object_meta_cnt_);
		}
		if(list_got_cnt > 0){
#ifdef _WIN32
			const char *marker1 = iflybox_lib_.iflybox_metadata_ownername()(tmp_metas[list_got_cnt - 1]);
			strcpy(marker, marker1);
			memcpy(total_tmp + object_meta_cnt_, tmp_metas, sizeof(ObjectMetaHandle) * list_got_cnt);
#else
			marker = iflybox_lib_.iflybox_metadata_ownername()(tmp_metas[list_got_cnt - 1]);
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
	proc_resp_mngr().add_nsec("iflybox_list_object", perf);

	if (status_code < 200 || status_code >= 300 )
	{
		dslog_error("iflybox_svc::on_iflybox_list_object | iflybox_list_object failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));

	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_list_object | iflybox_list_object success.");

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_list_object", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_accountmeta_create(const sc::par_arr& pars){
	last_used_accountmeta_ = iflybox_lib_.iflybox_accountmeta_create()();
	return 0;
}

int iflybox_svc::on_iflybox_bucketmeta_create(const sc::par_arr& pars){
	last_used_bucketmeta_ = iflybox_lib_.iflybox_bucketmeta_create()();
	return 0;
}

int iflybox_svc::on_iflybox_objectmeta_create(const sc::par_arr& pars){
	last_used_objectmeta_ = iflybox_lib_.iflybox_objectmeta_create()();
	return 0;
}

int iflybox_svc::on_iflybox_accountmeta_release(const sc::par_arr& pars){
	iflybox_lib_.iflybox_accountmeta_release()(last_used_accountmeta_);
	return 0;
}
int iflybox_svc::on_iflybox_reauth_account(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_reauth_account);
	int status_code = 0;

	dslog_perf_n(iflybox_reauth_account);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_reauth_account()(iflybox_handle_, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_reauth_account", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_reauth_account | iflybox_reauth_account failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));

	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_reauth_account | iflybox_reauth_account success.");

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_reauth_account", status_code, perf);
	return status_code;

}
int iflybox_svc::on_iflybox_bucketmeta_release(const sc::par_arr& pars){
	iflybox_lib_.iflybox_bucketmeta_release()(last_used_bucketmeta_);
	return 0;
}

int iflybox_svc::on_iflybox_objectmeta_release(const sc::par_arr& pars){
	iflybox_lib_.iflybox_objectmeta_release()(last_used_objectmeta_);
	return 0;
}

int iflybox_svc::on_iflybox_get_accountmeta(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_get_accountmeta);
	if(last_used_accountmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_get_accountmeta | AccountMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;

	dslog_perf_n(iflybox_get_accountmeta);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_get_accountmeta()(iflybox_handle_, last_used_accountmeta_, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_get_accountmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_get_accountmeta | iflybox_get_accountmeta failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_get_accountmeta");
	}
	iflybox_lib_.iflybox_release_result()(result);
	
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_get_accountmeta", status_code, perf);
	return status_code;
}
int iflybox_svc::on_iflybox_set_accountmeta(const par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_set_accountmeta);
	if(last_used_accountmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_set_accountmeta | AccountMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;

	dslog_perf_n(iflybox_set_accountmeta);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_set_accountmeta()(iflybox_handle_, last_used_accountmeta_, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_set_accountmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_set_accountmeta | iflybox_set_accountmeta failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_set_accountmeta");
	}
	iflybox_lib_.iflybox_release_result()(result);

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_set_accountmeta", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_remove_accountmeta(const par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_remove_accountmeta);
	if(last_used_accountmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_remove_accountmeta | AccountMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;

	dslog_perf_n(iflybox_remove_accountmeta);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_remove_accountmeta()(iflybox_handle_, last_used_accountmeta_, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_remove_accountmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_remove_accountmeta | iflybox_remove_accountmeta failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_remove_accountmeta");
	}
	iflybox_lib_.iflybox_release_result()(result);

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_remove_accountmeta", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_get_bucketmeta(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_get_bucketmeta);
	if(last_used_bucketmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_get_bucketmeta | BucketMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;
#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
#else
	const char* container = pars.first_par_str_value();
#endif
	dslog_perf_n(iflybox_get_bucketmeta);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_get_bucketmeta()(iflybox_handle_, container, last_used_bucketmeta_,result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_get_bucketmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_get_bucketmeta | iflybox_get_bucketmeta failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_get_bucketmeta | bucket = %s.", container);
	}
	
	iflybox_lib_.iflybox_release_result()(result);
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_get_bucketmeta", status_code, perf);
	return status_code;
}
int iflybox_svc::on_iflybox_set_bucketmeta(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_set_bucketmeta);
	if(last_used_bucketmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_set_bucketmeta | BucketMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;
#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
#else
	const char* container = pars.first_par_str_value();
#endif
	dslog_perf_n(iflybox_set_bucketmeta);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_set_bucketmeta()(iflybox_handle_, container, last_used_bucketmeta_,result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_set_bucketmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_set_bucketmeta | iflybox_set_bucketmeta failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_set_bucketmeta | bucket = %s.", container);
	}

	iflybox_lib_.iflybox_release_result()(result);
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_set_bucketmeta", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_remove_bucketmeta(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_remove_bucketmeta);
	if(last_used_bucketmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_remove_bucketmeta | BucketMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;
#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
#else
	const char* container = pars.first_par_str_value();
#endif
	dslog_perf_n(iflybox_remove_bucketmeta);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_remove_bucketmeta()(iflybox_handle_, container, last_used_bucketmeta_,result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_remove_bucketmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_remove_bucketmeta | iflybox_remove_bucketmeta failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_remove_bucketmeta | bucket = %s.", container);
	}

	iflybox_lib_.iflybox_release_result()(result);
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_remove_bucketmeta", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_get_objectmeta(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_get_objectmeta);
	if(last_used_objectmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_get_objectmeta | ObjectMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;
#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200];
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
	const char* objecttmp = pars.second_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* container = pars.first_par_str_value();
	const char* object = pars.second_par_str_value();
#endif
	dslog_perf_n(iflybox_get_objectmeta);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_get_objectmeta()(iflybox_handle_, container, object, last_used_objectmeta_, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_get_objectmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_get_objectmeta | iflybox_get_objectmeta failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_get_bucketmeta | bucket/object %s/%s", container, object);
	}
	iflybox_lib_.iflybox_release_result()(result);
	
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_get_objectmeta", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_set_objectmeta(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_set_objectmeta);
	if(last_used_objectmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_set_objectmeta | ObjectMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;
#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200]; 
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
	const char* objecttmp = pars.second_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* container = pars.first_par_str_value();
	const char* object = pars.second_par_str_value();
#endif
	
	dslog_perf_n(iflybox_set_objectmeta);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_set_objectmeta()(iflybox_handle_, container, object, last_used_objectmeta_, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_set_objectmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_set_objectmeta | iflybox_set_objectmeta failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_set_objectmeta | bucket/object %s/%s", container, object);
	}
	iflybox_lib_.iflybox_release_result()(result);

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_set_objectmeta", status_code, perf);
	return status_code;
}
/*
int iflybox_svc::on_iflybox_remove_objectmeta(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_remove_objectmeta);
	if(last_used_objectmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_remove_objectmeta | ObjectMetaHandle Invalid!");
		return -1;
	}
	int status_code = 0;
	const char* containertmp = pars.first_par_str_value();
	char container[1200]; 
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);
	const char* objecttmp = pars.second_par_str_value();
	char object[1200];
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
	dslog_perf_n(iflybox_remove_objectmeta);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_remove_objectmeta()(iflybox_handle_, container, object, last_used_objectmeta_, result);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_remove_objectmeta", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_remove_objectmeta | iflybox_remove_objectmeta failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_remove_bucketmeta | bucket/object %s/%s", container, object);
	}
	iflybox_lib_.iflybox_release_result()(result);

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_remove_objectmeta", status_code, perf);
	return status_code;
}
*/
int iflybox_svc::on_iflybox_metadata_set(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_metadata_set);

	int status_code = 0;
	const char* metatype = pars.first_par_str_value();
	MetadataHandle metas_hdl = NULL;
	if(metatype){
		if (strcmp(metatype, "account") == 0)
		{
			metas_hdl = last_used_accountmeta_;
		}
		if(strcmp(metatype, "bucket") == 0)
		{
			metas_hdl = last_used_bucketmeta_;
		}
		else if(strcmp(metatype, "object") == 0)
		{
			metas_hdl = last_used_objectmeta_;
		}
	}
	if(metas_hdl == NULL){
		dslog_error("iflybox_svc::on_iflybox_metadata_set | metatype error!");
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
	


	dslog_perf_n(iflybox_metadata_set);
//	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	status_code = iflybox_lib_.iflybox_metadata_set()(metas_hdl, metaname, metavalue);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_metadata_set", perf);

	if (status_code)
	{
		dslog_error("iflybox_svc::on_iflybox_metadata_set | iflybox_metadata_set failed. status_code = %d", status_code);
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_metadata_set | metaname/metavalue %s/%s", metaname, metavalue);
	}
//	iflybox_lib_.iflybox_release_result()(result);

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_metadata_set", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_metadata_get(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_metadata_get);

	int status_code = 0;
	const char* metatype = pars.first_par_str_value();
	MetadataHandle metas_hdl = NULL;
	if(metatype){
		if (strcmp(metatype, "account") == 0)
		{
			metas_hdl = last_used_accountmeta_;
		}
		if(strcmp(metatype, "bucket") == 0)
		{
			metas_hdl = last_used_bucketmeta_;
		}
		else if(strcmp(metatype, "object") == 0)
		{
			metas_hdl = last_used_objectmeta_;
		}
	}
	if(metas_hdl == NULL){
		dslog_error("iflybox_svc::on_iflybox_metadata_get | metatype error!");
		return -1;
	}
#ifdef _WIN32
	const char* metanametmp = pars.second_par_str_value();
	char metaname[1200]; 
	code_convert("GBK", "UTF-8", metanametmp, strlen(metanametmp), metaname, 1200);
#else
	const char* metaname = pars.second_par_str_value();
#endif
	dslog_perf_n(iflybox_metadata_get);
	//	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	const char* metavalue = iflybox_lib_.iflybox_metadata_get()(metas_hdl, metaname);
	if (metavalue)
		status_code = 0;
	else
		status_code = -1;
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_metadata_get", perf);

	if (status_code)
	{
		dslog_error("iflybox_svc::on_iflybox_metadata_get | iflybox_metadata_get failed. status_code = %d", status_code);
	}
	else{
		dslog_info("iflybox_svc::on_iflybox_metadata_get | metaname/metavalue %s/%s", metaname, metavalue);
	}
	//	iflybox_lib_.iflybox_release_result()(result);

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_metadata_get", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_accountmeta_bucketcount(const sc::par_arr& pars){
	if(last_used_accountmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_accountmeta_bucketcount | AccountMetaHandle Invalid!");
		return -1;
	}
	uint64_t bucketCount = iflybox_lib_.iflybox_accountmeta_bucketcount()(last_used_accountmeta_);
#ifdef _WIN32
	dslog_info("iflybox_svc::on_iflybox_get_bucketmeta | name = %s, bucketcount = %I64u.", iflybox_lib_.iflybox_metadata_ownername()(last_used_accountmeta_), bucketCount);
#elif _M_X64
	dslog_info("iflybox_svc::on_iflybox_get_bucketmeta | bucketcount = %lu.", bucketCount);
#elif _M_IX86
	dslog_info("iflybox_svc::on_iflybox_get_bucketmeta | bucketcount = %llu.", bucketCount);
#else
#error "Not found _WIN32 or _M_X64 or _M_IX86!"
#endif
	return 0;
}

int iflybox_svc::on_iflybox_accountmeta_bytesused(const sc::par_arr& pars){
	if(last_used_accountmeta_ == NULL){
		dslog_error("iflybox_svc::on_iflybox_accountmeta_bytesused | AccountMetaHandle Invalid!");
		return -1;
	}
	uint64_t bytesused = iflybox_lib_.iflybox_accountmeta_bytesused()(last_used_accountmeta_);
#ifdef _WIN32
	dslog_info("iflybox_svc::on_iflybox_accountmeta_bytesused | name = %s, bytesused = %I64u.", iflybox_lib_.iflybox_metadata_ownername()(last_used_accountmeta_), bytesused);
#elif _M_X64
	dslog_info("iflybox_svc::on_iflybox_accountmeta_bytesused | name = %s, bytesused = %lu.", iflybox_lib_.iflybox_metadata_ownername()(last_used_accountmeta_), bytesused);
#elif _M_IX86
	dslog_info("iflybox_svc::on_iflybox_accountmeta_bytesused | name = %s, bytesused = %llu.", iflybox_lib_.iflybox_metadata_ownername()(last_used_accountmeta_), bytesused);
#else
#error "Not found _WIN32 or _M_X64 or _M_IX86!"
#endif
	return 0;
}

int iflybox_svc::on_iflybox_bucketmeta_objectcount(const sc::par_arr& pars){
	return impl_iflybox_bucketmeta_objectcount(last_used_bucketmeta_);
}

int iflybox_svc::impl_iflybox_bucketmeta_objectcount(MetadataHandle handle){
	if(handle == NULL)
		dslog_error("iflybox_svc::on_iflybox_bucketmeta_objectcount | BucketMetaHandle Invalid!");
	uint64_t objectCount = iflybox_lib_.iflybox_bucketmeta_objectcount()(handle);
#ifdef _WIN32
	dslog_info("iflybox_svc::on_iflybox_bucketmeta_objectcount | name = %s, objectCount = %I64u.", iflybox_lib_.iflybox_metadata_ownername()(handle), objectCount);
#elif _M_X64
	dslog_info("iflybox_svc::on_iflybox_bucketmeta_objectcount | name = %s, objectCount = %lu.", iflybox_lib_.iflybox_metadata_ownername()(handle), objectCount);
#elif _M_IX86
	dslog_info("iflybox_svc::on_iflybox_bucketmeta_objectcount | name = %s, objectCount = %llu.", iflybox_lib_.iflybox_metadata_ownername()(handle), objectCount);
#else
#error "Not found _WIN32 or _M_X64 or _M_IX86!"
#endif
	return 0;
}

int iflybox_svc::on_iflybox_bucketmeta_bytesused(const sc::par_arr& pars){
	return impl_iflybox_bucketmeta_bytesused(last_used_bucketmeta_);
}

int iflybox_svc::impl_iflybox_bucketmeta_bytesused(MetadataHandle handle){
	if(handle == NULL){
		dslog_error("iflybox_svc::on_iflybox_bucketmeta_bytesused | BucketMetaHandle Invalid!");
		return -1;
	}
	uint64_t bytesused = iflybox_lib_.iflybox_bucketmeta_bytesused()(handle);
#ifdef _WIN32
	dslog_info("iflybox_svc::on_iflybox_bucketmeta_bytesused | name = %s, bytesused = %I64u.", iflybox_lib_.iflybox_metadata_ownername()(handle), bytesused);
#elif _M_X64
	dslog_info("iflybox_svc::on_iflybox_bucketmeta_bytesused | name = %s, bytesused = %lu.", iflybox_lib_.iflybox_metadata_ownername()(handle), bytesused);
#elif _M_IX86
	dslog_info("iflybox_svc::on_iflybox_bucketmeta_bytesused | name = %s, bytesused = %llu.", iflybox_lib_.iflybox_metadata_ownername()(handle), bytesused);
#else
#error "Not found _WIN32 or _M_X64 or _M_IX86!"
#endif
	return 0;
}

int iflybox_svc::on_iflybox_objectmeta_contentlength(const sc::par_arr& pars){
	return impl_iflybox_objectmeta_contentlength(last_used_objectmeta_);
}


int iflybox_svc::impl_iflybox_objectmeta_contentlength(MetadataHandle handle){
	if(handle == NULL){
		dslog_error("iflybox_svc::on_iflybox_objectmeta_contentlength | ObjectMetaHandle Invalid!");
		return -1;
	}
	uint64_t congtentlength = iflybox_lib_.iflybox_objectmeta_contentlength()(handle);
#ifdef _WIN32
	dslog_info("iflybox_svc::on_iflybox_objectmeta_contentlength | name = %s, congtentlength = %I64u.", iflybox_lib_.iflybox_metadata_ownername()(handle), congtentlength);
#elif _M_X64
	dslog_info("iflybox_svc::on_iflybox_objectmeta_contentlength | name = %s, congtentlength = %lu.", iflybox_lib_.iflybox_metadata_ownername()(handle), congtentlength);
#elif _M_IX86
	dslog_info("iflybox_svc::on_iflybox_objectmeta_contentlength | name = %s, congtentlength = %llu.", iflybox_lib_.iflybox_metadata_ownername()(handle), congtentlength);
#else
#error "Not found _WIN32 or _M_X64 or _M_IX86!"
#endif
	return 0;
}

int iflybox_svc::on_iflybox_objectmeta_etag(const sc::par_arr& pars){
	return impl_iflybox_objectmeta_etag(last_used_objectmeta_);
}

int iflybox_svc::impl_iflybox_objectmeta_etag(MetadataHandle handle){
	if(handle == NULL){
		dslog_error("iflybox_svc::on_iflybox_objectmeta_etag | ObjectMetaHandle Invalid!");
		return -1;
	}
	const char* etag = iflybox_lib_.iflybox_objectmeta_etag()(handle);
	dslog_info("iflybox_svc::on_iflybox_objectmeta_etag | name = %s, etag = %s.", iflybox_lib_.iflybox_metadata_ownername()(handle), etag);
	return 0;
}

int iflybox_svc::on_iflybox_objectmeta_lastmodify(const sc::par_arr& pars){
	return impl_iflybox_objectmeta_lastmodify(last_used_objectmeta_);
}

int iflybox_svc::impl_iflybox_objectmeta_lastmodify(MetadataHandle handle){
	if(handle == NULL){
		dslog_error("iflybox_svc::on_iflybox_objectmeta_lastmodify | ObjectMetaHandle Invalid!");
		return -1;
	}
	const char* lastmodify = iflybox_lib_.iflybox_objectmeta_lastmodify()(handle);
	dslog_info("iflybox_svc::on_iflybox_objectmeta_lastmodify | name = %s, lastmodify = %s.", iflybox_lib_.iflybox_metadata_ownername()(handle), lastmodify);
	return 0;
}





int iflybox_svc::on_loop_metadata(const sc::par_arr& pars){
	const char* metatype = pars.first_par_str_value();
	MetadataHandle* loop_metas = NULL;
	int loop_size = 0;
	if(metatype){
		if(strcmp(metatype, "bucket") == 0){
			loop_metas = container_metas_;
			loop_size = container_meta_cnt_;
		}
		else if(strcmp(metatype, "object") == 0){
			loop_metas = object_metas_;
			loop_size = object_meta_cnt_;
		}
	}
	if(loop_metas == NULL){
		dslog_error("iflybox_svc::on_loop_metadata | metatype error!");
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
				dslog_error("iflybox_svc::on_loop_metadata | proc %s not exist!", proc_name.c_str());
				continue;
			}
			
			(this->*proc_point)(loop_metas[index]);
			
		}
	}
	dslog_info("iflybox_svc::on_loop_metadata | loop count = %d.", loop_size);
	return 0;
}


int iflybox_svc::on_iflybox_multipart_upload_init(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_multipart_upload_init);
#ifdef _WIN32
	const char* containertmp = pars.first_par_str_value();
	char container[1200]; 
	code_convert("GBK", "UTF-8", containertmp, strlen(containertmp), container, 1200);

	const char* objecttmp = pars.second_par_str_value();
	char object[1200]; 
	code_convert("GBK", "UTF-8", objecttmp, strlen(objecttmp), object, 1200);
#else
	const char* container = pars.first_par_str_value();

	const char* object = pars.second_par_str_value();
#endif

	int status_code = 0;

	dslog_perf_n(iflybox_multipart_upload_init);
	
	status_code = iflybox_lib_.iflybox_multipart_upload_init()(iflybox_handle_, container, object, last_upload_id_);
	double perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_multipart_upload_init", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_multipart_upload_init | iflybox_multipart_upload_init failed. status_code = %d.", status_code);
	}
	dslog_verbose("iflybox_svc::on_iflybox_multipart_upload_init | iflybox_multipart_upload_init success.");
	dslog_info("iflybox_svc::on_iflybox_multipart_upload_init | %s/%s UploadId = %s.", container, object, last_upload_id_);
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_multipart_upload_init", status_code, perf);
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
int iflybox_svc::on_iflybox_multipart_upload_part(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_multipart_upload_part);
	int partnum = pars.first_par_int_value(0, 0);
	const char* srcpath = pars.second_par_str_value();
	int offset = pars.third_par_int_value(0, 0);
	uint64_t length = pars.fourth_par_int_value(0, 0);
	const char* md5 = pars.fifth_par_str_value();
	int status_code = 0;
	double perf = 0;

	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
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
		dslog_perf_n(iflybox_object_exist);
		status_code = iflybox_lib_.iflybox_multipart_upload_part()(iflybox_handle_, last_upload_id_, partnum, multipart_callback/*iflybox_lib_.iflybox_put_object_fromdisk_callback()*/, &minput, 0, md5, result);
		perf = dslog_perf_stop();
		fclose(fd);
		proc_resp_mngr().add_nsec("iflybox_multipart_upload_part", perf);
	}else{
		status_code = -2;
	}


	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_multipart_upload_part | iflybox_multipart_upload_part failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_multipart_upload_part | iflybox_multipart_upload_part success.");
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_multipart_upload_part", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_multipart_upload_list_parts(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_multipart_upload_list_parts);

	
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


	dslog_perf_n(iflybox_multipart_upload_list_parts);
	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	object_meta_cnt_ = 0;
	do{
		const int list_wanted_cnt = (limit - object_meta_cnt_) > MAX_LIMIT ? MAX_LIMIT : (limit - object_meta_cnt_);
		int list_got_cnt = list_wanted_cnt;
		ObjectMetaHandle* tmp_metas = (list_got_cnt >= 0) ? new ObjectMetaHandle[list_got_cnt] : NULL;
		status_code = iflybox_lib_.iflybox_multipart_upload_list_parts()(iflybox_handle_, last_upload_id_, tmp_metas, &list_got_cnt, marker, result);
		ObjectMetaHandle* total_tmp = (list_got_cnt >= 0) ? new ObjectMetaHandle[object_meta_cnt_ + list_got_cnt] : NULL;
		if(object_meta_cnt_)
		{
			memcpy(total_tmp, object_metas_, sizeof(ObjectMetaHandle) * object_meta_cnt_);
		}
		if(list_got_cnt > 0){
#ifdef _WIN32
			const char *marker1 = iflybox_lib_.iflybox_metadata_ownername()(tmp_metas[list_got_cnt - 1]);
			strcpy(marker, marker1);
#else
            marker = iflybox_lib_.iflybox_metadata_ownername()(tmp_metas[list_got_cnt - 1]);
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
	proc_resp_mngr().add_nsec("iflybox_multipart_upload_list_parts", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_multipart_upload_list_parts | iflybox_multipart_upload_list_parts failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));

	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_multipart_upload_list_parts | iflybox_multipart_upload_list_parts success.");

	AutoRecordUnitError(proc_resp_mngr(), "iflybox_multipart_upload_list_parts", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_multipart_upload_complete(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_multipart_upload_complete);
	int status_code = 0;
	double perf = 0;

	iflyboxResult result = iflybox_lib_.iflybox_init_result()();
	
	dslog_perf_n(iflybox_object_exist);
	status_code = iflybox_lib_.iflybox_multipart_upload_complete()(iflybox_handle_, last_upload_id_, result);
	perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_multipart_upload_complete", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_multipart_upload_complete | iflybox_multipart_upload_complete failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_multipart_upload_complete | iflybox_multipart_upload_complete success.");
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_multipart_upload_complete", status_code, perf);
	return status_code;
}

int iflybox_svc::on_iflybox_multipart_upload_abort(const sc::par_arr& pars){
	dslog_tracer(iflybox_svc::on_iflybox_multipart_upload_abort);
	int status_code = 0;
	double perf = 0;

	iflyboxResult result = iflybox_lib_.iflybox_init_result()();

	dslog_perf_n(iflybox_object_exist);
	status_code = iflybox_lib_.iflybox_multipart_upload_abort()(iflybox_handle_, last_upload_id_, result);
	perf = dslog_perf_stop();
	proc_resp_mngr().add_nsec("iflybox_multipart_upload_abort", perf);

	if (status_code < 200 || status_code >= 300)
	{
		dslog_error("iflybox_svc::on_iflybox_multipart_upload_abort | iflybox_multipart_upload_abort failed. status_code = %d, error message:%s", status_code, iflybox_lib_.iflybox_result_detail()(result));
	}
	iflybox_lib_.iflybox_release_result()(result);
	dslog_verbose("iflybox_svc::on_iflybox_multipart_upload_abort | iflybox_multipart_upload_abort success.");
	AutoRecordUnitError(proc_resp_mngr(), "iflybox_multipart_upload_abort", status_code, perf);
	return status_code;
}
