#pragma once

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <stdexcept>

#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "hiredis.lib")

#include <winsock.h>
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4200)
#include <hiredis.h>
#pragma warning(pop)
#else
#include <hiredis.h>
#endif

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN( typeName )	\
	typeName( const typeName & );		\
	typeName & operator=( const typeName & )
#endif

namespace RedisCpp
{
	typedef std::list<std::string> ValueList;
	typedef std::map<std::string, std::string> ValueMap;

	// ������ ��� ���̳� ������ ��� �ڿ� ����
	enum INSERT_POS
	{
		BEFORE,
		AFTER,
	} E_INSERT_POS;

	class CRedisConn
	{
	public:
		CRedisConn(void)
		{
			_redCtx = NULL;
			_host.clear();
			_password.clear();
			_port = 0;
			_timeout = 0;
			_connected = false;
			_errStr = _errDes[ERR_NO_ERROR];
		}

		virtual ~CRedisConn(void)
		{
			disConnect();
		}

		/**
		 *@brief ���� ���� �ʱ�ȭ
		 *
		 *������ �ʱ�ȭ �ϴµ� ��� �Ǵ� ���� ����. connect() ���� ����Ѵ�. ���� ȣ�� ����
		 *bool connect( const std::string &host ,const uint16_t port, const std::string& password , const uint32_t timeout );
		 */
		void init(const std::string& host = "127.0.0.1", const uint16_t port = 6379,
			const std::string& password = "", const uint32_t timeout = 0)
		{
			_host = host;
			_port = port;
			_password = password;
			_timeout = timeout;
		}


		/**
		 *@brief ���ӡ�redis �����ͺ��̽�
		 *
		 * �� �޼ҵ�� �����ͺ��̽��� �����ϱ� ���� init() �Ŀ� ȣ���Ѵ�.
		 *
		 *@return ������true, ���С�false
		 */
		bool connect()
		{
			if (_connected)
			{
				disConnect();
			}

			struct timeval timeoutVal;
			timeoutVal.tv_sec = _timeout;
			timeoutVal.tv_usec = 0;

			_redCtx = redisConnectWithTimeout(_host.c_str(), _port, timeoutVal);
			if (_getError(_redCtx))
			{
				if (NULL != _redCtx)
				{
					redisFree(_redCtx);
					_redCtx = NULL;
				}
				_connected = false;
				return false;
			}

			_connected = true;
			return true;

			// if connection  need password
		/*
			if ( _password == "" )
			{
				return true;
			}
			else
			{
				return ( auth( _password ) );   //< ���� ���е� false ��ȯ
			}
		*/
		}

		/**
		 *@brief ���� ����. Redis �� �޸� �����ͺ��̽�
		 */
		void disConnect()
		{
			if (_connected && NULL != _redCtx)
			{
				redisFree(_redCtx);
				_redCtx = NULL;
			}
			_connected = false;
		}

		/**
		 *@brief redis ������ ���̽��� ������ �� ����
		 *
		 *@param [in] �α��� ��� ��ȣ
		 *@return ������true, ���С�false.
		 */
		bool auth(const std::string& password)
		{
			if (!_connected)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			_password = password;
			redisReply* reply = static_cast<redisReply*>(redisCommand(_redCtx, "auth %s",
				_password.c_str()));

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				ret = true;
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}
			return ret;
		}

		/**
		 *@brief ����
		 *
		 * �ʱ�ȭ�ϴµ� �ʿ��� ����. ��й�ȣ�� ��� ���� ������ ���� �ڵ����� ����ȴ�.
		 *
		 *@param [in]  host .  redis-server�� ip��
		 *@param [in] port .redis-server�� port ��
		 *@param [in] password . redis-server�� ��й�ȣ. �� �Ű� ������ ��� �� �� ������, �Է����� ������ ������ �������� �ʴ´�.
		 *@param [in] timeout .���� �ð� �ʰ�
		 *@return ���� true�������� false
		 */
		bool connect(const std::string& host, const uint16_t port, const std::string& password =
			"", const uint32_t timeout = 0)
		{
			// Init attribute.
			init(host, port, password, timeout);

			return (connect());
		}

		/**
		 * @brief ���� ���� ����
		 *
		 * @return ���� �Ǿ� �ִٸ� true�� ��ȯ�ϰ�, �׷��� ������ false�� ��ȯ.
		 */
		bool ping()
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			redisReply* reply = static_cast<redisReply*>(redisCommand(_redCtx, "PING"));
			bool ret = false;

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				ret = true;
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}

		/**
		 *@brief redis�� �ٽ� ����
		 *
		 *@return �� ���� ������true, �� ���� ���С�false.
		 */
		bool reconnect()
		{
			return (connect());
		}

		/**
		 * @brief redis�� ���� ����
		 *
		 * @return ���� �Ǿ� �ִٸ顡true, �ƴϸ顡false.
		 */
		inline bool isConneced()
		{
			return _connected;
		}

		/**
		 * @brief ���� ����
		 *
		 * ��return ���� ���� ��ȯ
		 */
		const std::string getErrorStr() const
		{
			return _errStr;
		}

		/**
		 * @brief ��� �Ű� ������ redis ������ ���� ������.
		 *
		 * ��param [in] format
		 * ��param [in] ... ��� �� ������ ���ڿ�
		 * @return NULL, redis ���� ���� �� ������ ������ ���. �����̶�顡redisReply ��ȯ
		 */
		redisReply* redisCmd(const char* format, ...)
		{
			va_list ap;
			va_start(ap, format);
			redisReply* reply = static_cast<redisReply*>(redisvCommand(_redCtx, format, ap));
			va_end(ap);
			return reply;
		}

		///////////////////////////////// list ���� /////////////////////////////////////
		/**
		 * @brief list ���ʿ� ����
		 * @param [in] retval ���� ���� �� list ����
		 * @return ���� false������ true
		 */
		bool lpush(const std::string& key, const std::string& value, uint64_t& retval)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			retval = 0;
			bool ret = false;

			redisReply* reply = redisCmd("LPUSH %s %s", key.c_str(), value.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				retval = reply->integer;
				ret = true;
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}
			return ret;

		}

		/**
		 * @brief list ���ʿ��� ����
		 * @param [in] value ���� ��
		 * @return ���� false������ true
		 */
		bool lpop(const std::string& key, std::string& value)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("LPOP %s", key.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				// ����
				if (NULL == reply->str)
				{
					_errStr = _errDes[ERR_NO_KEY];
					value = "";
					ret = false;
				}
				else
				{
					value = reply->str;
					ret = true;
				}
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}

		/**
		 * @brief ������ ������ list�� �ִ� ��Ҹ� �����´�.
		 * @param [in] start ���� ���� ���Σ�stop ���� �� ����, valueList ���� list
		 */
		bool lrange(const std::string& key, uint32_t start, int32_t end, ValueList& valueList)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("LRANGE %s %d %d", key.c_str(), start, end);

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				if (REDIS_REPLY_ARRAY == reply->type && 0 == reply->elements) //<  key�� list ���� Ÿ�� start > end
				{
					_errStr = std::string(_errDes[ERR_INDEX]) + " or "
						+ _errDes[ERR_NO_KEY];
					ret = false;

				}
				else
				{
					_getArryToList(reply, valueList);
					ret = true;
				}
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}

		/**
		 * @brief list �����ʿ� ����
		 * @param [in] retval ���� ���� ���� list ��� ��
		 * @return ���� false������ true
		 */
		bool rpush(const std::string& key, const std::string& value, uint64_t& retval)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			retval = 0;
			bool ret = false;

			redisReply* reply = redisCmd("RPUSH %s %s", key.c_str(), value.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				retval = reply->integer;
				ret = true;
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}

		/**
		 * @brief list �����ʿ��� ������
		 * @param [in] value ���� ��
		 * @return ���� false������ true
		 */
		bool rpop(const std::string& key, std::string& value)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("RPOP %s", key.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				// ����
				if (NULL == reply->str)
				{
					_errStr = _errDes[ERR_NO_KEY];
					value = "";
					ret = false;
				}
				else
				{
					value = reply->str;
					ret = true;
				}
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}

		/**
		 * @brief LINSERT
		 * list�� Ư�� ��ġ�� ������ �ֱ�
		 */
		bool linsert(const std::string& key, INSERT_POS position, const std::string& pivot,
			const std::string& value, int64_t& retval)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			std::string pos;

			if (BEFORE == position)
			{
				pos = "BEFORE";
			}
			else if (AFTER == position)
			{
				pos = "AFTER";
			}

			bool ret = false;
			redisReply* reply = redisCmd("LINSERT %s %s %s %s", key.c_str(), pos.c_str(),
				pivot.c_str(), value.c_str());

			if (_getError(reply))	//< list ������ �ƴ�
			{
				ret = false;
			}
			else
			{
				if (REDIS_REPLY_INTEGER == reply->type)
				{
					if (reply->integer == -1)
					{
						_errStr = _errDes[ERR_NO_PIVOT];
						ret = false;
					}
					else if (reply->integer == 0)
					{
						_errStr = _errDes[ERR_NO_KEY];
						ret = false;
					}
					else
					{
						retval = reply->integer;
						ret = true;
					}
				}
				else
				{
					ret = false;
				}
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}

		/**
		 * @brief LINDEX
		 * lindex key index
		 * index�� ��ġ�� �ִ� �����͸� ��ȸ�Ѵ�. 0 ���� �����Ѵ�
		 */
		bool lindex(const std::string& key, int32_t index, std::string& value)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("LINDEX %s %d", key.c_str(), index);

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				// ����
				if (REDIS_REPLY_NIL == reply->type)
				{
					_errStr = std::string(_errDes[ERR_NO_KEY]) + " or " +
						_errDes[ERR_INDEX];
					value = "";
					ret = false;
				}
				else
				{
					value = reply->str;
					ret = true;
				}
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}

		/**
		 * @brief LLEN
		 * list���� value�� ������ ��ȸ https://kwoncharlie.blog.me/220397447626
		 */
		bool llen(const std::string& key, uint64_t& retval)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("LLEN %s", key.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				if (REDIS_REPLY_INTEGER == reply->type && (0 == reply->integer))
				{
					_errStr = _errDes[ERR_NO_KEY];
					ret = false;
				}
				else
				{
					retval = reply->integer;
					ret = true;
				}
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}




		//////////////////////////////  string ��� ��� //////////////////////////////////////

		bool get(const std::string& key, std::string& value)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("GET %s", key.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				if (REDIS_REPLY_NIL == reply->type)
				{
					_errStr = std::string(_errDes[ERR_NO_KEY]) + " or " + _errDes[ERR_NO_FIELD];
					ret = false;
				}
				else
				{
					value = reply->str;
					ret = true;
				}
			}
			if (NULL != reply)
			{
				freeReplyObject(reply);
			}
			else
			{
			}

			return ret;
		}

		bool set(const std::string& key, const std::string& value, uint32_t& retval)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("SET %s %s", key.c_str(), value.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				retval = (uint32_t)reply->integer;
				ret = true;
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}

		bool del(const std::string& key, uint32_t& retval)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("DEL %s", key.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				retval = (uint32_t)reply->integer;
				ret = true;
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}


		//////////////////////////////   hash ��� ��� //////////////////////////////////////

		/**
		 * @brief �ؽ� ���̺��� Ű�� �ʵ忡 �ش��ϴ� ���� �����´�.
		 * @param [in] key�� ���̺� �̸��� ���� Ű �̸��̴�
		 * @param [in] filed �ʵ� �̸�
		 * @param [out] value ���� ��
		 * @return ���� true������ false
		 * @warning ���� ���� value""(string �ʱ�ȭ ��)
		 */
		bool hget(const std::string& key, const std::string& filed, std::string& value)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("HGET %s %s", key.c_str(), filed.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				if (REDIS_REPLY_NIL == reply->type)
				{
					_errStr = std::string(_errDes[ERR_NO_KEY]) + " or " +
						_errDes[ERR_NO_FIELD];
					ret = false;
				}
				else
				{
					value = reply->str;
					ret = true;
				}
			}
			if (NULL != reply)
			{
				freeReplyObject(reply);
			}
			else
			{

			}

			return ret;
		}


		/**
		 * @brief �ؽ� ���̺��� Ű�� �ʵ��� ���� �����Ѵ�
		 * @param [in] key ���̺� �̸��� ���� Ű �̸��̴�
		 * @param [in] filed �ʵ� �̸�
		 * @param [in] value ���� �� �Ű� ������ �ش��ϴ� ��
		 * @param [out] retval 0:field�� �����ϰ� ���� ����٣� 1�� field�� �������� �ʰ� �� �ʵ尡 �����ǰ� ���� ���� �ȴ�.
		 * @return ���� true������ false
		 * @warning ���� retval�� 0�������̶�� 1
		 */
		bool hset(const std::string& key, const std::string& filed, const std::string& value, uint32_t& retval)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("HSET %s %s %s", key.c_str(), filed.c_str(), value.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{
				retval = (uint32_t)reply->integer;
				ret = true;
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}

		/**
		 * @brief �ؽ� ���̺��� Ű�� �ش��ϴ� �ʵ带 �����Ѵ�.
		 * @param [in] key ���̺� �̸��� ���� Ű �̸�
		 * @param [in] filed �ʵ� �̸�
		 * @param [out] retval�������� �ʵ� ��
		 * @return ���� true������ false
		 *@warning ���д� retval 0�������̶�� 1
		 */
		bool hdel(const std::string& key, const std::string& filed, uint32_t& retval)
		{
			{
				if (!_connected || !_redCtx)
				{
					_errStr = _errDes[ERR_NO_CONNECT];
					return false;
				}

				bool ret = false;
				redisReply* reply = redisCmd("HDEL %s %s", key.c_str(), filed.c_str(), retval);

				if (_getError(reply))
				{
					ret = false;
				}
				else
				{
					if (REDIS_REPLY_INTEGER == reply->type && 0 == reply->integer)
					{
						_errStr = std::string(_errDes[ERR_NO_KEY]) + " or " + _errDes[ERR_NO_FIELD];
					}
					//std::cout<<"type = "<<reply->type<<"   integer = "<< reply->integer<<std::endl;
					//std::cout<<"str = " << reply->str<<std::endl;
					else
					{
						retval = (uint32_t)reply->integer;
						ret = true;
					}
				}

				if (NULL != reply)
				{
					freeReplyObject(reply);
				}

				return ret;
			}
		}

		/**
		 * @brief �ؽ� ���̺��� Ű�� �ش��ϴ� ��� ������ �����´�
		 * @param [in] key ���̺� �̸��� ���� Ű �̸��̴�.
		 * @param [out] valueMap ����map<string,string> ������
		 * @return ���� true������ false
		 * @warning �����̸� valueMap�� ��� �ִ�
		 */
		bool hgetall(const std::string& key, ValueMap& valueMap)
		{
			if (!_connected || !_redCtx)
			{
				_errStr = _errDes[ERR_NO_CONNECT];
				return false;
			}

			bool ret = false;
			redisReply* reply = redisCmd("HGETALL %s", key.c_str());

			if (_getError(reply))
			{
				ret = false;
			}
			else
			{

				if (REDIS_REPLY_ARRAY == reply->type && 0 == reply->elements)
				{
					_errStr = _errDes[ERR_NO_KEY];

				}
				else
				{
					_getArryToMap(reply, valueMap);
					ret = true;
				}
			}

			if (NULL != reply)
			{
				freeReplyObject(reply);
			}

			return ret;
		}

	protected:
		/**
		 *@brief REDIS_REPLY_ARRY ������ reply->type ��ҿ��� �����͸� ���� �ͼ� valueList ��Ͽ� ä���.
		 *
		 *@param [in] reply  . the data return from redis-server.
		 *@param [out] valueList. reply���� ����� ���� ������
		 *@return ���� true, ���С�false.
		 */
		bool _getArryToList(redisReply* reply, ValueList& valueList)
		{
			if (NULL == reply)
			{
				return false;
			}

			std::size_t num = reply->elements;

			for (std::size_t i = 0; i < num; i++)
			{
				valueList.push_back(reply->element[i]->str);
			}

			//	ValueList::iterator it = valueList.begin();
			//
			//	for( ; it != valueList.end(); it++ )
			//	{
			//		std::cout << "valueList: "<< *it << std::endl;
			//	}
			return true;
		}

		/**
		 *@brief REDIS_REPLY_ARRY ������ reply->type ��ҿ��� �����͸� ���� �ͼ� valueMap ����� ä���.
		 *
		 *hgetall�� Ȧ���� �ʵ� �̸�����, ¦���� �ʵ� �����ͷ� ��ȯ�Ѵ�. �ʵ� �̸� = �ʵ� �����ͷ� valueMap�� ����ȴ�.
		 *
		 *@param [in] reply  . the data return from redis-server.
		 *@param [out] valueMap. reply���� ����� ���� ��� �����ʹ� valueMap�� ����ȴ�.
		 *@return ���� true, ���С�false.
		 */
		bool _getArryToMap(redisReply* reply, ValueMap& valueMap)
		{
			if (NULL == reply)
			{
				return false;
			}

			std::size_t num = reply->elements;

			for (std::size_t i = 0; i < num; i += 2)
			{
				valueMap.insert(
					std::pair<std::string, std::string>(reply->element[i]->str,
						reply->element[i + 1]->str));
			}

			return true;
		}

		bool _getError(const redisContext* redCtx)
		{
			_errStr = _errDes[ERR_NO_ERROR];
			if (redCtx == NULL)
			{
				_errStr = _errDes[ERR_NULL];
				return true;
			}
			if (redCtx->err != 0)
			{
				_errStr = redCtx->errstr;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool _getError(const redisReply* reply)
		{
			_errStr = _errDes[ERR_NO_ERROR];
			if (reply == NULL)
			{
				_errStr = _errDes[ERR_NULL];
				return false;
			}

			// have error
			if (reply->type == REDIS_REPLY_ERROR)
			{
				_errStr = reply->str;
				return true;
			}
			else
			{
				return false;
			}
		}

		inline redisContext* _getCtx() const
		{
			return _redCtx;
		}

	private:
		DISALLOW_COPY_AND_ASSIGN(CRedisConn);

		redisContext* _redCtx;		///< redis connector context

		std::string _host;         		///< redis host
		uint16_t _port;         		///< redis sever port
		std::string _password;         	///< redis server password
		uint32_t _timeout;      		///< connect timeout second
		bool _connected;			///< if connected

		///< error number
		enum E_ERROR_NO
		{
			ERR_NO_ERROR = 0,
			ERR_NULL,
			ERR_NO_CONNECT,
			ERR_NO_PIVOT,
			ERR_NO_KEY,
			ERR_NO_FIELD,
			ERR_INDEX,
			ERR_BOTTOM
		};
		std::string _errStr;		///< Describe the reason for error..

		static const char* _errDes[ERR_BOTTOM];	///< describe error
	};

	const char* CRedisConn::_errDes[ERR_BOTTOM] =
	{
			"No error.",
			"NULL pointer ",
			"No connection to the redis server.",
					"Inser Error,pivot not found.",
					"key not found",
			"hash field not found",
			"error index"
	};

} /* namespace RedisCpp */


