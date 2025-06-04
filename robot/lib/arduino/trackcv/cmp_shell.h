/*!
	\file
	\brief Файл с описанием класса CMP_Shell, реализующего оболочку для работы с C MsgPack
*/

#ifndef cmp_shellH
#define cmp_shellH

#include "cmp.h"
#include <stdint.h>
#include <stdio.h>

#include "log.hpp"

//#pragma pack(push, 1)

/*!
	Класс CMP_Shell, реализующий оболочку для работы с C MsgPack.
	Работает с буфером, указатель на который получает при инициализации (функция Init).

	Пример использования при разборе полученного пакета: смотри ProcessRequestStreamDataPacket из emp_messages.cpp
	Пример использования при формировании отправляемого пакета: смотри MakeResponseStaticInfo из emp_messages.cpp
*/
class CMP_Shell {
private:
	uint8_t *buffer; ///< указатель на буфер, из которого читаем / в который записываем структуру msgpack
	uint32_t buffer_size; ///< размер буфера
	uint32_t write_pos; ///< позиция записи в буфере
	uint32_t read_pos; ///< позиция чтения в буфере
	cmp_ctx_t cmp; ///< структура msgpack, используфется функциями из cmp.cpp
	Logger* log;
public:
	CMP_Shell() {
	}
	
	void init(const char* mode, uint8_t *buffer, uint32_t buffer_size, Logger& log);
	
	/// сбросить позицию чтения
	void ResetReadPosition() {
		read_pos = 0;
	}
	
	/// сбросить позицию чтения и позицию записи
	void Reset() {
		write_pos = 0;
		read_pos = 0;
	}
	
	/*!
		\brief Получить размер буфера
		\return Возвращает размер буфера
	*/
	uint32_t getBufferSize() {
		return buffer_size;
	}
	
	/*!
		\brief Получить указатель буфера
		\return Возвращает размер буфера
	*/
	uint8_t* getBuffer() {
		return buffer;
	}
	
	/*!
		\brief Получить размер записанных в буфер данных (msgpack структуры)
		\return Возвращает размер записанных в буфер данных (msgpack структуры)
	*/
	uint32_t getSize() {
		return write_pos;
	}
	
	/*!
		\brief Установить размер msgpack структуры в буфере (позицию записи)
		\details Вызывается перед чтением msgpack структуры, чтобы объект "знал" какой размер структуры msgpack расположен в буфере
		\param [in] size Размер msgpack структуры в буфере
		\return Возвращает true если устанавливаемый размер msgpack структуры в буфере не превышает размер буфера, иначе false
	*/	
	bool SetSize(uint32_t size) {
		if (size > buffer_size) {
			return false;
		}
		write_pos = size;
		return true;
	}
	
	/*!
		\brief Получить позицию чтения
		\return Возвращает позицию чтения
	*/
	uint32_t GetReadPos() {
		return read_pos;
	}
	
	/*!
		\brief Получить указатель на место в буфере, соответствующее позиции чтения
		\return Возвращает указатель на место в буфере, соответствующее позиции чтения
	*/
	uint8_t* uint8_tptr_read_pos() {
		return (buffer + read_pos);
	}
	
	/*!
		\brief Получить указатель на cmp структуру 
		\return Возвращает указатель на cmp структуру
	*/
	cmp_ctx_t* cmp_ptr() {
		if (buffer == NULL) {
			return NULL;
		}
		return &cmp;
	}

	/*!
		\brief Прочитать данные из буфера
		\details Функция читает указанное количество байт из буфера
		
		\param [in] data Указатель, по которому нужно записать прочитаные данные
		\param [in] bytes_to_read Количество байт, которое нужно прочитать
		
		\return Возвращает true если в буфере имеется указанное количество байт (от текущей позиции чтения до конца данных, т.е. до позиции записи)
	*/
	bool read_bytes(uint8_t *data, uint32_t bytes_to_read) {
		log->debug("read %d bytes from %d (up to %d)\n", bytes_to_read, read_pos, write_pos);
		if (buffer == NULL) {
			log->error("buffer is NULL\n");
			return false;
		}
		if (data == NULL) {
			log->error("data is NULL\n");
			return false;
		}
		if (read_pos + bytes_to_read > write_pos) {
			log->error("not enough data\n");
			return false;
		}
		
		memcpy(data, buffer + read_pos, bytes_to_read);
		read_pos += bytes_to_read;
		
		return true;
	}
	
	/*!
		\brief Записать данные в буфер
		\details Функция записывает в буфер указанное количество байт
		
		\param [in] data Указатель на данные, которые нужно записать в буфер
		\param [in] bytes_to_write Количество байт, которое нужно записать в буфер
		
		\return Возвращает размер записанных в буфер данных (0 если в буфере не хватило места для записи данных)
	*/
	uint32_t write_bytes(uint8_t *data, uint32_t bytes_to_write) {
		if (buffer == NULL || data == NULL || write_pos + bytes_to_write > buffer_size) {
			return 0;
		}
		memcpy(buffer + write_pos, data, bytes_to_write);
		write_pos += bytes_to_write;
		return bytes_to_write;
	}
	/*
	bool write_bytes_from_ringbuffer(EMP_RingBuffer *rb, uint32_t bytes_to_write) {
		if (buffer == NULL || rb == NULL || write_pos + bytes_to_write > buffer_size) {
			return 0;
		}
		if (!cmp_write_bin_marker(&cmp, bytes_to_write)) return false;
		if (!rb->ReadAndDeleteData(buffer + write_pos, bytes_to_write)) {
			return false;
		}
		write_pos += bytes_to_write;
		return true;
	}*/

	/*!
		\brief Записать данные в буфер
		\details Функция записывает в буфер указанное количество байт
		
		\param [in] data Указатель на данные, которые нужно записать в буфер
		\param [in] bytes_to_write Количество байт, которое нужно записать в буфер
		
		\return Возвращает true в случае успешного выполнения операции.
		Возвращает false если в структуре msgpack находящейся в буфере больше нет msgpack объектов расположенных после позиции чтения (прочитали все объекты msgpack)
	*/
	bool read_object(cmp_object_t *obj) {
		if (buffer == NULL || obj == NULL) return false;
		return cmp_read_object(&cmp, obj);
	}
};


#endif
