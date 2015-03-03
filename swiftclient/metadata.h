#ifndef __CSSP__METADATA_H__
#define __CSSP__METADATA_H__


#include <stdint.h>
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace cssp
{


//metadata»ùÀà
class Metadata{
public:
	virtual const char* getMeta(const std::string& metaname) = 0;
	virtual void setMeta(const std::string& metaname, const std::string& metavalue) = 0;
	virtual int			countMeta() = 0;
	virtual const std::string& ownerName(){return owner_name_;}
public:
	void setOwnerName(const std::string& ownername){owner_name_ = ownername;}
public:
	virtual void parse(const HttpHeaderMap& header) = 0;
private:
	std::string		owner_name_;
};

//account metadata
class AccountMetadata : public Metadata{
public:
	const char*	getMeta(const std::string& metaname);
	void	setMeta(const std::string& metaname, const std::string& metavalue);
	int			countMeta();
	void parse(const HttpHeaderMap& header);
	void clearMeta();
public:
	uint64_t	countContainer();
	uint64_t	countObject();
	uint64_t	bytesUsed();
public:
	MetadataMap	account_meta_;
	MetadataMap	account_sysdef_;
public:
	static	std::string		account_prefix_;
	static	std::string		account_remove_prefix_;
};

//container metadata
class ContainerMetadata : public Metadata{
public:
	const char* getMeta(const std::string& metaname);
	void setMeta(const std::string& metaname, const std::string& metavalue);
	int			countMeta();
	void parse(const HttpHeaderMap& header);
	void clearMeta();
public:
	uint64_t	countObject();
	uint64_t	bytesUsed();
public:
	MetadataMap	container_meta_;
	MetadataMap	container_sysdef_;
public:
	static	std::string		container_prefix_;
	static	std::string		container_remove_prefix_;
};

//object metadata
class ObjectMetadata : public Metadata{
public:
	const char* getMeta(const std::string& metaname);
	void setMeta(const std::string& metaname, const std::string& metavalue);
	int			countMeta();
	void parse(const HttpHeaderMap& header);
	void clearMeta();
public:
	bool		isPseudoDirectory();
	uint64_t	contentLength();
	const char*	lastModified();
	const char*	Etag();
public:
	MetadataMap	object_meta_;
	MetadataMap	object_sysdef_;
private:
	bool		pseudo_dir_;
public:
	static	std::string		object_prefix_;
	static	std::string		object_remove_prefix_;
};

}

#endif // __CSSP__METADATA_H__
