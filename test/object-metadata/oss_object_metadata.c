/*
 * =============================================================================
 *
 *       Filename:  oss_object_metadata.c
 *
 *    Description:  object metadata structure and implementation
 *
 *        Created:  09/05/2012 02:44:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <assert.h>
#include <stdio.h>

#include "oss_object_metadata.h"


//#define _OSS_OBJECT_METADATA_H
//#include <ossc/oss_object_metadata.h>
//#undef _OSS_OBJECT_METADATA_H

/* *
 * 添加一个用户自定义的元数据
 * */
static inline void 
_add_user_metadata(oss_object_metadata_t *metadata,
		const char *key,
		const char *value)
{
	oss_map_t *oss_map = metadata->user_metadata;
	oss_map_put(oss_map, key, value);
}

/* *
 * 获取Cache-Control请求头，表示用户指定的HTTP请求/回复链的缓存行为
 * */
static inline const char *
_get_cache_control(oss_object_metadata_t *metadata)
{
	char *buf;
	int result;
	oss_map_t *oss_map = metadata->metadata;

	result = oss_map_get(oss_map, OSS_CACHE_CONTROL, NULL, 0);
	if (result == 0)
		return NULL;
	buf = (char *)malloc(sizeof(char) * result);
	memset(buf, result, 0);
	result = oss_map_get(oss_map, OSS_CACHE_CONTROL, buf, result);
	if (result == 0)
		return NULL;
	return buf;
}

/* *
 * 获取Content-Disposition请求头，表示MIME用户代理如何显示附加的文件
 * */
static inline const char *
_get_content_disposition(oss_object_metadata_t *metadata)
{
	char *buf;
	int result;
	oss_map_t *oss_map = metadata->metadata;

	result = oss_map_get(oss_map, OSS_CONTENT_DISPOSITION, NULL, 0);
	if (result == 0)
		return NULL;
	buf = (char *)malloc(sizeof(char) * result);
	memset(buf, result, 0);
	result = oss_map_get(oss_map, OSS_CONTENT_DISPOSITION, buf, result);
	if (result == 0)
		return NULL;
	return buf;
}

/* *
 * 获取Content-Encoding请求头，表示Object内容的编码方式
 * */
static inline const char *
_get_content_encoding(oss_object_metadata_t *metadata)
{
	char *buf;
	int result;
	oss_map_t *oss_map = metadata->metadata;

	result = oss_map_get(oss_map, OSS_CONTENT_ENCOING, NULL, 0);
	if (result == 0)
		return NULL;
	buf = (char *)malloc(sizeof(char) * result);
	memset(buf, result, 0);
	result = oss_map_get(oss_map, OSS_CONTENT_ENCOING, buf, result);
	if (result == 0)
		return NULL;
	return buf;
}

/* *
 * 获取Content-Length请求头，表示Object内容的大小
 * */
static inline long
_get_content_length(oss_object_metadata_t *metadata)
{
	char *buf;
	int result;
	int len;
	oss_map_t *oss_map = metadata->metadata;

	result = oss_map_get(oss_map, OSS_CONTENT_LENGTH, NULL, 0);
	if (result == 0)
		return 0;
	buf = (char *)malloc(sizeof(char) * result);
	memset(buf, result, 0);
	result = oss_map_get(oss_map, OSS_CONTENT_LENGTH, buf, result);
	if (result == 0)
		return 0;
	len = atoi(buf);
	free(buf);
	return len;
}

/* *
 * 获取Content-Type请求头，表示Object内容的类型，为标准的MIME类型
 * */
static inline const char *
_get_content_type(oss_object_metadata_t *metadata)
{
	char *buf;
	int result;
	oss_map_t *oss_map = metadata->metadata;

	result = oss_map_get(oss_map, OSS_CONTENT_TYPE, NULL, 0);
	if (result == 0)
		return NULL;
	buf = (char *)malloc(sizeof(char) * result);
	memset(buf, result, 0);
	result = oss_map_get(oss_map, OSS_CONTENT_TYPE, buf, result);
	if (result == 0)
		return NULL;
	return buf;
}

/* *
 * 获取一个值表示与Object相关的hex编码的128位MD5摘要
 * */

static inline const char *
_get_etag(oss_object_metadata_t *metadata)
{
	char *buf;
	int result;
	oss_map_t *oss_map = metadata->metadata;

	result = oss_map_get(oss_map, OSS_CONTENT_MD5, NULL, 0);
	if (result == 0)
		return NULL;
	buf = (char *)malloc(sizeof(char) * result);
	memset(buf, result, 0);
	result = oss_map_get(oss_map, OSS_CONTENT_MD5, buf, result);
	if (result == 0)
		return NULL;
	return buf;
}

/* *
 * 获取Expires请求头
 * */
static inline const char *
_get_expiration_time(oss_object_metadata_t *metadata)
{
	char *buf;
	int result;
	oss_map_t *oss_map = metadata->metadata;

	result = oss_map_get(oss_map, OSS_EXPIRES, NULL, 0);
	if (result == 0)
		return NULL;
	buf = (char *)malloc(sizeof(char) * result);
	memset(buf, result, 0);
	result = oss_map_get(oss_map, OSS_EXPIRES, buf, result);
	if (result == 0)
		return NULL;
	return buf;
}

/* *
 * 获取Last-Modified请求头的值，表示Object最后一次修改的时间
 * */
static inline const char *
_get_last_modified(oss_object_metadata_t *metadata)
{
	char *buf;
	int result;
	oss_map_t *oss_map = metadata->metadata;

	result = oss_map_get(oss_map, OSS_LAST_MODIFIED, NULL, 0);
	if (result == 0)
		return NULL;
	buf = (char *)malloc(sizeof(char) * result);
	memset(buf, result, 0);
	result = oss_map_get(oss_map, OSS_LAST_MODIFIED, buf, result);
	if (result == 0)
		return NULL;
	return buf;
}

/* *
 * 返回内部保存的请求头的元数据（内部使用）
 * */
static inline oss_map_t *
_get_raw_metadata(oss_object_metadata_t *metadata)
{
	oss_map_t *oss_map = metadata->metadata;
	return oss_map;
}

/* *
 * 获取用户自定义的元数据
 * */
static inline oss_map_t *
_get_user_metadata(oss_object_metadata_t *metadata)
{
	oss_map_t *oss_map = metadata->user_metadata;
	return oss_map;
}

/* *
 * 设置Cache-Control请求头，表示用户指定的HTTP请求/回复链的缓存行为
 * */
static inline void
_set_cache_control(oss_object_metadata_t *metadata, 
		const char *cache_control)
{
	oss_map_t *oss_map = metadata->metadata;
	oss_map_put(oss_map, OSS_CACHE_CONTROL, cache_control);
}

/* *
 * 设置Content-Disposition请求头，表示MIME用户代理如何显示附加的文件
 * */
static inline void
_set_content_disposition(oss_object_metadata_t *metadata,
		const char *disposition)
{
	oss_map_t *oss_map = metadata->metadata;
	oss_map_put(oss_map, OSS_CONTENT_DISPOSITION, disposition);
}

/* *
 * 设置Content-Encoding请求头，表示Object内容的编码方式
 * */
static inline void
_set_content_encoding(oss_object_metadata_t *metadata,
		const char *encoding)
{
	oss_map_t *oss_map = metadata->metadata;
	oss_map_put(oss_map, OSS_CONTENT_ENCOING, encoding);
}

/* *
 *  设置Content-Length请求头，表示Object内容的大小
 * */
static inline void
_set_content_length(oss_object_metadata_t *metadata,
		long content_length)
{
	char content_len[32];
	sprintf(content_len, "%ld", content_length);
	oss_map_t *oss_map = metadata->metadata;
	oss_map_put(oss_map, OSS_CONTENT_LENGTH, content_len);
}

/* *
 * 获取Content-Type请求头，表示Object内容的类型，为标准的MIME类型
 * */
static inline void
_set_content_type(oss_object_metadata_t *metadata,
		const char *content_type)
{
	oss_map_t *oss_map = metadata->metadata;
	oss_map_put(oss_map, OSS_CONTENT_TYPE, content_type);
}

/* *
 *  设置Expires请求头
 * */
static inline void
_set_expiration_time(oss_object_metadata_t *metadata,
		const char *expiration_time)
{
	oss_map_t *oss_map = metadata->metadata;
	oss_map_put(oss_map, OSS_EXPIRES, expiration_time);
}

/* *
 *  设置请求头（内部使用）
 * */
static inline void
_set_header(oss_object_metadata_t *metadata,
		const char *key,
		const char *value)
{
	oss_map_t *oss_map = metadata->metadata;
	oss_map_put(oss_map, key, value);
}

/* *
 * 设置Last-Modified请求头的值，表示Object最后一次修改的时间（内部使用）
 * */
static inline void
_set_last_modified(oss_object_metadata_t *metadata,
		const char *last_modified)
{
	oss_map_t *oss_map = metadata->metadata;
	oss_map_put(oss_map, OSS_LAST_MODIFIED, last_modified);
}

/* *
 * 设置用户自定义的元数据，表示以x-oss-meta-为前缀的请求头
 * */
static inline void
_set_user_metadata(oss_object_metadata_t *metadata,
		oss_map_t *user_metadata)
{
	oss_map_t *oss_map = metadata->user_metadata;
	oss_map_delete(oss_map);
	oss_map = user_metadata;
}

/* *
 * 初始化构造函数
 * */
oss_object_metadata_t *
object_metadata_initialize()
{
	oss_object_metadata_t *metadata;
	metadata = (oss_object_metadata_t *)malloc(sizeof(oss_object_metadata_t));
	metadata->metadata = oss_map_new(64);
	metadata->user_metadata = oss_map_new(64);

	metadata->add_user_metadata       = _add_user_metadata;
	metadata->get_cache_control       = _get_cache_control;
	metadata->get_content_disposition = _get_content_disposition;
	metadata->get_content_encoding    = _get_content_encoding;
	metadata->get_content_length      = _get_content_length;
	metadata->get_content_type        = _get_content_type;
	metadata->get_etag                = _get_etag;
	metadata->get_expiration_time     = _get_expiration_time;
	metadata->get_last_modified       = _get_expiration_time;
	metadata->get_raw_metadata        = _get_raw_metadata;
	metadata->get_user_metadata       = _get_user_metadata;

	metadata->set_cache_control       = _set_cache_control;
	metadata->set_content_disposition = _set_content_disposition;
	metadata->set_content_encoding    = _set_content_encoding;
	metadata->set_content_length      = _set_content_length;
	metadata->set_content_type        = _set_content_type;
	metadata->set_expiration_time     = _set_expiration_time;
	metadata->set_header              = _set_header;
	metadata->set_last_modified       = _set_last_modified;
	metadata->set_user_metadata       = _set_user_metadata;

	return metadata;
}

/* *
 * 析构函数
 * */
void
object_metadata_finalize(oss_object_metadata_t *metadata)
{

	assert(metadata != NULL);

	if (metadata != NULL) {
		if (metadata->metadata != NULL) {
			free(metadata->metadata);
			metadata->metadata = NULL;
		}
		if (metadata->user_metadata != NULL) {
			free(metadata->user_metadata);
			metadata->user_metadata = NULL;
		}
		free(metadata);
		metadata = NULL;
	}
}
