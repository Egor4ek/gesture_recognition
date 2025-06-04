#include "cmp_shell.h"

// msgpack need this functions for read and write data
static bool cmp_shell_reader(cmp_ctx_t *ctx, void *data, size_t limit);
static size_t cmp_shell_writer(cmp_ctx_t *ctx, const void *data, size_t count);

/*!
	\brief Чтение данных (используется функциями msgpack'а)
	\details Функция создана по примеру с сайта msgpack. 
		Функция читает данные структуры msgpack в хранилище (в данном случае в буфере объекта CMP_Shell)
	
	\param [in] ctx Указатель на стркутуру cmp, используемую функциями cmp.cpp
	\param [in] data Указатель на буфер, в который читаем данные
	\param [in] limit Количество данных, которое необходимо прочитать
	
	\return Возвращает true если прочитано указанное количество данных, иначе false (если данных меньше или структура msgpack повреждена)
*/
static bool cmp_shell_reader(cmp_ctx_t *ctx, void *data, size_t limit) {
    CMP_Shell *cmp_shell = (CMP_Shell*)ctx->buf;
    return cmp_shell->read_bytes((uint8_t*)data, limit);
}

/*!
	\brief Запись данных (используется функциями msgpack'а)
	\details Функция создана по примеру с сайта msgpack. 
		Функция записывает данные, сгенерированные функциями msgpack в хранилище (в данном случае в буфер объекта CMP_Shell)
	
	\param [in] ctx Указатель на стркутуру cmp, используемую функциями cmp.cpp
	\param [in] data Указатель на буфер с данными
	\param [in] count Размер данных
	
	\return Возвращает количество записанных данных
*/
static size_t cmp_shell_writer(cmp_ctx_t *ctx, const void *data, size_t count) {
    CMP_Shell *cmp_shell = (CMP_Shell*)ctx->buf;
    return cmp_shell->write_bytes((uint8_t*)data, count);
}

/*!
\brief Инициализация объекта CMP_Shell
\details Функция сохраняет в параметры объекта указатель на рабочий буфер и его размер, а также инициализирует cmp - стркутуру используемую функциями cmp.cpp

\param [in] buffer Указатель на буфер содержащий msgpack структуру либо на буфер в который хотим записать msgpack стркутуру
\param [in] buffer_size Размер буфера

\return Возвращает true в случае успешного выполнения операции.
Возвращает false если входные параметры некорректны (нулевой указатель на буфер или нулевой размер буфера)
*/

void CMP_Shell::init(const char* mode, uint8_t *buffer, uint32_t buffer_size, Logger& log) {
	memset(this, 0x00, sizeof(CMP_Shell));
	if (buffer == NULL || buffer_size == 0) {
		return;
	}
	Reset();
	this->buffer = buffer;
	this->buffer_size = buffer_size;
	
	if(strcmp(mode, "r") == 0) {
		SetSize(buffer_size);
	} else if(strcmp(mode, "w") == 0) {
		SetSize(0);
	} else {
		log.error("unknown mode\n");
	}
	
	cmp_init(&cmp, this, cmp_shell_reader, cmp_shell_writer);
	
	this->log = &log;
}

