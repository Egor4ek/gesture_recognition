#ifndef HOST_LIB_H
#define HOST_LIB_H

#include <stdint.h>

#include "trackcv/imagenet_const.h"

/*!
* \file
* Библиотека работы с модулем компьютерного зрения trackCV
*/

/*! \defgroup face Распознавание лиц
 * @{
*/

/*!
 * \brief Запуск анализатора лиц
 * 
 * Запуск также производится при первом запросе информации о лицах.
 * 
 * 
*/

void trackcv_face_start();


/*!
 * \brief Остановка анализатора
 * 
 * Остановка анализатора также производится при запуске другого режима распознавания.
 * 
*/

void trackcv_face_stop();

/*!
 * \brief Количество найденых лиц
 * 
 * Возвращает количество лиц в кадре.
 * При обнаружении нового лица, ему присваивается порядковый номер (начиная с 0).
 * При пропадании лица из кадра номера лиц пересчитываются для сохранения непрерывной индексации.
 * Максимальное количество определяемых лиц — 5, в порядке их попадания в кадр;
 * 
 * \return количество найденных лиц. Значение по-умолчанию: 0
*/

uint32_t trackcv_face_count();

/*!
 * \brief Координата X найденного лица
 * 
 * Для найденного лица возвращает координату X центра описывающего прямоугольника в процентах от ширины кадра.
 * Если лицо с заданным индексом не найдено -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер лица
 * 
 * \return величина координаты X. Значение по-умолчанию: 0
*/

uint32_t trackcv_face_x(uint32_t index);

/*!
 * \brief Координата Y найденного лица
 * 
 * Для найденного лица возвращает координату Y центра описывающего прямоугольника в процентах от высоты кадра.
 * Если лицо с заданным индексом не найдено -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер лица
 * 
 * \return величина координаты Y. Значение по-умолчанию: 0
*/

uint32_t trackcv_face_y(uint32_t index);

/*!
 * \brief Ширина найденного лица
 * 
 * Для найденного лица возвращает ширину описывающего прямоугольника в процентах от ширины кадра.
 * Если лицо с заданным индексом не найдено -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер лица
 * 
 * \return величина ширины лица. Значение по-умолчанию: 0
*/

uint32_t trackcv_face_width(uint32_t index);

/*!
 * \brief Высота найденного лица
 * 
 * Для найденного лица возвращает высоту описывающего прямоугольника в процентах от высоты кадра.
 * Если лицо с заданным индексом не найдено -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер лица
 * 
 * \return величина высоты лица. Значение по-умолчанию: 0
*/

uint32_t trackcv_face_height(uint32_t index);

/*! @} */

/*! \defgroup line Отслеживание направляющей линии
 * 
 * Выполняется поиск контрастной направляющей линии на однотонном фоне.
 * 
 * * Линия распознается методом выделения контура в рамке высотой *LINE_HEIGHT* (по-умолчанию, 5%), находящейся на середине высоты кадра.
 * * Линией считается контур, шириной не более *LINE_MIN_WIDTH* (по-умолчанию, 20%) от ширины кадра.
 * * При обнаружении нескольких линий, алгоритм выполняет распознавание их всех.
 * * Для каждой линии координата определяется как усредненное посредством медианного фильтра за заданное *FRAME_FILTER*
 * (по-умолчанию, 10) количество кадров значение координаты X центра контура.
 * * Решение о наличии в кадре линии и количестве линий принимается, если контур обнаруживается не менее чем *LINE_MIN_FILL*
 * (по-умолчанию, 40%) времени.
 * 
 * @{
*/

/*!
 * \brief Запуск анализатора линии
 * 
 * Запуск также производится при первом запросе информации.
 * 
*/

void trackcv_line_start();


/*!
 * \brief Остановка анализатора
 * 
 * Остановка анализатора также производится при запуске другого режима распознавания. 
 * 
*/

void trackcv_line_stop();

/*!
 * \brief Количество найденых линий
 * 
 * Возвращает количество линий в кадре.
 * 
 * \return количество найденных линий. Значение по-умолчанию: 0
*/

uint32_t trackcv_line_count();

/*!
 * \brief Смещение X найденной линии
 * 
 * Для найденной линии возвращает смещение относительно центра кадра. в процентах от ширины кадра.
 * Для смещения влево — меньше 0, для смещения вправо — больше 0.
 * Если линия с заданным индексом не найдена -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер линии
 * 
 * \return смещение по координате X. Значение по-умолчанию: 0
*/

int32_t trackcv_line_x(uint32_t index);

/*!
 * \brief Ширина найденной линии
 * 
 * Для найденной линии возвращает ширину в процентах от ширины кадра.
 * Если линия с заданным индексом не найдена -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер линии
 * 
 * \return величина ширины линии. Значение по-умолчанию: 0
*/

uint32_t trackcv_line_width(uint32_t index);

/*!
 * \brief Установить высоту рамки для выделения контура. 
 * 
 * Устанавливает параметр LINE_HEIGHT. Значение по-умолчанию: 5%
 * 
 * \param line_height высота рамки
*/

void trackcv_set_line_height(uint32_t line_height);

/*!
 * \brief Установить максимальную ширину контура.
 * 
 * Устанавливает параметр LINE_MIN_WIDTH. Значение по-умолчанию: 20%
 * 
 * \param line_min_width максимальная ширина контура
*/

void trackcv_set_line_min_width(uint32_t line_min_width);

/*!
 * \brief Установить количество кадров для усреднения.
 * 
 * Устанавливает параметр FRAME_FILTER. Значение по-умолчанию: 10
 * 
 * \param frame_filter количество кадров для усреднения
*/

void trackcv_set_frame_filter(uint32_t frame_filter);

/*!
 * \brief Установить процент времени, в течение которого должна обнаруживаться линия.
 * 
 * Устанавливает параметр LINE_MIN_FILL. Значение по-умолчанию: 40%
 * 
 * \param line_min_fill процент времени, в течение которого должна обнаруживаться линия
*/

void trackcv_set_line_min_fill(uint32_t line_min_fill);


/*! @} */

/*! \defgroup square Обнаружение прямоугольников
 * 
 * Режим не реализован
 * @{
*/

/*! @} */

/*! \defgroup circle Обнаружение окружностей
 * @{
*/

/*!
 * \brief Запуск анализатора окружностей
 * 
 * Запуск также производится при первом запросе информации.
 * 
 * 
*/

void trackcv_circle_start();


/*!
 * \brief Остановка анализатора
 * 
 * Остановка анализатора также производится при запуске другого режима распознавания.
 * 
*/

void trackcv_circle_stop();

/*!
 * \brief Количество найденых окружностей
 * 
 * Возвращает количество окружностей в кадре.
 * 
 * \return количество найденных окружностей. Значение по-умолчанию: 0
*/

uint32_t trackcv_circle_count();

/*!
 * \brief Координата X найденной окружности
 * 
 * Для найденной окружности возвращает координату X центра в процентах от ширины кадра.
 * Если окружность с заданным индексом не найдена -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер окружности
 * 
 * \return величина координаты X. Значение по-умолчанию: 0
*/

uint32_t trackcv_circle_x(uint32_t index);

/*!
 * \brief Координата Y найденной окружности
 * 
 * Для найденной окружности возвращает координату Y центра в процентах от высоты кадра.
 * Если окружность с заданным индексом не найдена -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер окружности
 * 
 * \return величина координаты Y. Значение по-умолчанию: 0
*/

uint32_t trackcv_circle_y(uint32_t index);

/*!
 * \brief Радиус найденной окружности
 * 
 * Для найденной окружности возвращает радиусв процентах от среднеквадратичного значения высоты и ширины кадра.
 * Если окружность с заданным индексом не найдена -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер окружности
 * 
 * \return величина радиуса. Значение по-умолчанию: 0
*/

uint32_t trackcv_circle_R(uint32_t index);

/*! @} */

/*! \defgroup qr Распознавание QR-кодов
 * @{
*/

/*!
 * \brief Запуск анализатора QR-кодов
 * 
 * Запуск также производится при первом запросе информации.
 * 
*/

void trackcv_qr_start();

/*!
 * \brief Остановка анализатора
 * 
 * Остановка анализатора также производится при запуске другого режима распознавания. 
 * 
*/

void trackcv_qr_stop();

/*!
 * \brief Флаг наличия QR кода
 * 
 * Возвращает флаг наличия QR кода
 * 
 * \return true, если найден QR-код, иначе false
*/

bool trackcv_qr_found();


/*!
 * \brief Получить содержимое QR-кода
 * 
 * Возвращает содержимое QR-кода
 * 
 * \return указатель на нуль-терминированную строку
*/

char* trackcv_qr_data();

/*!
 * \brief Получить интервал времени с момента распознавания QR-кода
 * 
 * 
 * \return интервал времени в секундах
*/
uint32_t trackcv_qr_interval();

/*! @} */

/*! \defgroup motion Поиск движения
 * @{
*/

/*!
 * \brief Запуск анализатора движения
 * 
 * Запуск также производится при первом запросе информации.
 * 
 * 
*/

void trackcv_motion_start();


/*!
 * \brief Остановка анализатора
 * 
 * Остановка анализатора также производится при запуске другого режима распознавания.
 * 
*/

void trackcv_motion_stop();

/*!
 * \brief Количество найденых областей
 * 
 * Возвращает количество областей в кадре, в которых обнаружено движение.
 * 
 * \return количество найденных областей. Значение по-умолчанию: 0
*/

uint32_t trackcv_motion_count();

/*!
 * \brief Координата X найденной области
 * 
 * Для найденной области возвращает координату X центра описывающего прямоугольника в процентах от ширины кадра.
 * Если область с заданным индексом не найдена -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер области
 * 
 * \return величина координаты X. Значение по-умолчанию: 0
*/

uint32_t trackcv_motion_x(uint32_t index);

/*!
 * \brief Координата Y найденной области
 * 
 * Для найденной области возвращает координату Y центра описывающего прямоугольника в процентах от высоты кадра.
 * Если область с заданным индексом не найдена -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер области
 * 
 * \return величина координаты Y. Значение по-умолчанию: 0
*/

uint32_t trackcv_motion_y(uint32_t index);

/*!
 * \brief Ширина найденной области
 * 
 * Для найденной области возвращает ширину описывающего прямоугольника в процентах от ширины кадра.
 * Если область с заданным индексом не найдена -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер области
 * 
 * \return величина ширины области. Значение по-умолчанию: 0
*/

uint32_t trackcv_motion_width(uint32_t index);

/*!
 * \brief Высота найденной области
 * 
 * Для найденной области возвращает высоту описывающего прямоугольника в процентах от высоты кадра.
 * Если область с заданным индексом не найдена -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер области
 * 
 * \return величина высоты области. Значение по-умолчанию: 0
*/

uint32_t trackcv_motion_height(uint32_t index);

/*! @} */

/*! \defgroup color Анализ цветовой гаммы кадра
 * @{
*/

/*!
 * \brief Запуск анализатора цветов
 * 
 * Запуск также производится при первом запросе информации.
 * 
*/

void trackcv_color_start();


/*!
 * \brief Остановка анализатора
 * 
 * Остановка анализатора также производится при запуске другого режима распознавания. 
 * 
*/

void trackcv_color_stop();

/// Коды возможных цветов
typedef enum {
	aqua = 0,	///< Бирюзовый
	black = 1,	///< Черный
	blue = 2,	///< Синий
	gray = 4,	///< Серый
	green = 5,	///< Зеленый
	purple = 10,///< Пурпурный
	red = 11,	///< Красный
	white = 14,	///< Белый
	yellow = 15	///< Желтый
} Color_code;

/*!
 * \brief Величина угла цвета
 * 
 * Возвращает доминирующий цвет в кадре
 * 
 * \return hue, в диапазоне 0..360
*/

uint32_t trackcv_color_hue();

/*!
 * \brief Величина насыщенности цвета
 * 
 * Возвращает насыщенность доминирующего цвета в кадре
 * 
 * \return saturation, в процентах
*/
uint32_t trackcv_color_saturation();

/*!
 * \brief Величина яркости
 * 
 * Возвращает яркость кадра
 * 
 * \return value, в процентах
*/
uint32_t trackcv_color_value();

/*!
 * \brief Величина кода цвета
 * 
 * Возвращает код доминирующего цвета в кадре
 * 
 * \return код цвета
*/
uint32_t trackcv_color_code();

/*! @} */

/*! \defgroup neural Нейроинтерфейс
 * 
 * TODO
 * 
 * @{
*/

/// Идентификаторы скриптов
typedef enum {
	Neural_script_id_dummy = 0,
	Neural_script_id_mnist = 1,
	Neural_script_id_imagenet = 2,
	Neural_script_id_text_detection = 3,
	Neural_script_id_face_detection = 4,
	Neural_script_id_head_pose = 5,
	Neural_script_id_age_gender = 6,
	Neural_script_id_emotion = 7, ///< Распознавание эмоций
	Neural_script_id_gesture = 8,
	Neural_script_id_landmarks_5 = 9,
	Neural_script_id_landmarks_35 = 10,
	Neural_script_id_face_recognition = 11,
    Neural_script_id_custom_binary_classifier = 12,
    Neural_script_id_custom_categorical_classifier = 13,
	Neural_script_id_speech = 14,
	Neural_script_id_speech_eng = 15
} Neural_script_id;

typedef enum {
	Neural_gesture_UP = 1,
	Neural_gesture_DOWN = 2,
	Neural_gesture_LEFT = 3,
	Neural_gesture_RIGHT = 4,
	Neural_gesture_OPEN = 5,
	Neural_gesture_CLOSE = 6,
	Neural_gesture_STOP = 7
} Neural_gesture;

typedef enum {
	Neural_emotion_NEUTRAL = 0,
	Neural_emotion_HAPPY = 1,
	Neural_emotion_SAD = 2,
	Neural_emotion_SURPRISE = 3,
	Neural_emotion_ANGER = 4
} Neural_emotion;


/*!
 * \brief Запуск скрипта
 * 
 * Запуск также производится при первом запросе информации.
 * 
*/

void trackcv_neural_start(Neural_script_id script_id);


/*!
 * \brief Остановка скрипта
 * 
 * Остановка анализатора также производится при запуске другого режима распознавания. 
 * 
*/

void trackcv_neural_stop();

/*!
 * \brief Количество найденых объектов
 * 
 * Возвращает количество объектов в кадре.
 * 
 * \return количество найденных объектов. Значение по-умолчанию: 0
*/

uint32_t trackcv_neural_count();

/*!
 * \brief Смещение X найденного объекта
 * 
 * Для найденного объекта возвращает смещение описывающего прямоугольника, в процентах от ширины кадра.
 * Если объект с заданным индексом не найден -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер объекта
 * 
 * \return смещение по координате X. Значение по-умолчанию: 0
*/

uint32_t trackcv_neural_x(uint32_t index);

/*!
 * \brief Смещение Y найденного объекта
 * 
 * Для найденного объекта возвращает смещение описывающего прямоугольника, в процентах от ширины кадра.
 * Если объект с заданным индексом не найден -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер объекта
 * 
 * \return смещение по координате Y. Значение по-умолчанию: 0
*/

uint32_t trackcv_neural_y(uint32_t index);

/*!
 * \brief Ширина найденного объекта
 * 
 * Для найденного объекта возвращает ширину в процентах от ширины кадра.
 * Если объект с заданным индексом не найден -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер объекта
 * 
 * \return величина ширины объекта. Значение по-умолчанию: 0
*/

uint32_t trackcv_neural_width(uint32_t index);

/*!
 * \brief Высота найденного объекта
 * 
 * Для найденного объекта возвращает высоту в процентах от высоты кадра.
 * Если объект с заданным индексом не найден -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер объекта
 * 
 * \return величина высоты объекта. Значение по-умолчанию: 0
*/

uint32_t trackcv_neural_height(uint32_t index);

/*!
 * \brief Количество классов найденного объекта
 * 
 * Для найденного объекта возвращает количество задетектированных классов.
 * Если объект с заданным индексом не найден -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер объекта
 * 
 * \return количество классов. Значение по-умолчанию: 0
*/

uint32_t trackcv_neural_class_count(uint32_t index);

/*!
 * \brief Идентификатор класса
 * 
 * Для найденного объекта и класса возвращает идентификатор.
 * Если класс с заданным индексом не найден -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер объекта
 * \param class_index порядковый номер класса
 * 
 * \return идентификатор класса. Значение по-умолчанию: 0
*/

uint32_t trackcv_neural_class_id(uint32_t index, uint32_t class_index);

/*!
 * \brief Вероятность класса
 * 
 * Для найденного объекта и класса возвращает вероятность в процентах.
 * Если класс с заданным индексом не найден -- возвращает значение по-умолчанию.
 * 
 * \param index порядковый номер объекта
 * \param class_index порядковый номер класса
 * 
 * \return вероятность класса. Значение по-умолчанию: 0
*/

uint32_t trackcv_neural_class_p(uint32_t index, uint32_t class_index);

/*!
 * \brief Метаинформация класса
 * 
 * Для найденного объекта и класса возвращает указатель на строку с метаинформацией
 * Если класс с заданным индексом не найден -- возвращает 0
 * 
 * \param index порядковый номер объекта
 * \param class_index порядковый номер класса
 * 
 * \return указатель на строку. Значение по-умолчанию: 0
*/
char* trackcv_neural_class_meta(uint32_t index, uint32_t class_index);
char* trackcv_neural_class_text();

/*!
 * \brief Сохранить лицо
 * 
 * Сохранить лицо. Должно быть одно в кадре
 * \param char имя файла. Дополняется .png кладется в папку.
 *
 * \return флаг успешности
*/
bool trackcv_neural_save_face(char* name);

/*!
 * \brief Удалить все сохраненные лица
 * 
 * Сохранить лицо. Должно быть одно в кадре
 * \param char имя файла. Дополняется .png кладется в папку.
 *
 * \return флаг успешности
*/
bool trackcv_neural_clear_faces();

/*! @} */

/*! \defgroup common Общие функции
 * @{
*/

typedef enum {
	ERR_OK = 0, // нормальная работа
	WRONG_INDEX = 1, // неправильный индекс массива
	NO_MODULE = 2, // нет ответа от модуля
	MODULE_ERR = 3, // ошибка модуля
	COMM_ERR = 4, // ошибка библиотеки связи
	MODULE_NOREADY = 5 // модуль не готов
} Errno;

/*!
 * \brief Получить код ошибки
 * 
 * После осуществления обращения к переменной пользователь может получить информацию, выполнилась ли функция корректно,
 * или выполнилась с ошибкой и вернула значение по-умолчанию.
 * 
 * \return тип Errno
*/

Errno trackcv_get_errno();

/*!
 * \brief Инициализировать подключение к модулю
 * 
 * Работа с модулем должна начинаться с вызова этой функции
 * 
 * \param reader callback-функция для чтения из порта
 * \param writer callback-функция для записи в порт
 * \param logger логгер
 * 
 * \return true в случае ответа модуля, false в случае ошибки (код ошибки в errno)
*/

bool Trackcv_init(int32_t (*reader)(void), bool (*writer)(char), int32_t (*putfunc)(unsigned char));

/*!
 * \brief Опрос модуля
 * 
 * Проверка доступности и работоспособности модуля
 * 
 * \return true в случае ответа модуля, false в случае ошибки (код ошибки в errno)
*/

bool trackcv_check(void);


/*!
 * \brief Установить левую границу кадра
 * 
 * Устанавливает границу кадра для анализа. Значение по-умолчанию: 0.
 * 
 * \param frame_x координата x левой границы кадра (в процентах относительно ширины кадра)
*/

void trackcv_set_frame_x(uint32_t frame_x);

/*!
 * \brief Установить нижнюю границу кадра
 * 
 * Устанавливает границу кадра для анализа. Значение по-умолчанию: 0.
 * 
 * \param frame_y координата y нижней границы кадра (в процентах относительно высоты кадра)
*/

void trackcv_set_frame_y(uint32_t frame_y);

/*!
 * \brief Установить высоту кадра
 * 
 * Устанавливает высоту кадра для анализа. Значение по-умолчанию: 0.
 * 
 * \param frame_height высота кадра (в процентах относительно высоты кадра видеопотока)
*/

void trackcv_set_frame_height(uint32_t frame_height);

/*!
 * \brief Установить ширину кадра
 * 
 * Устанавливает ширину кадра для анализа. Значение по-умолчанию: 0.
 * 
 * \param frame_width ширина кадра (в процентах относительно ширины кадра видеопотока)
*/

void trackcv_set_frame_width(uint32_t frame_width);

/*! @} */
#endif
