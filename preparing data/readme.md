#### Инструкция по подготовке излюражений

1. Заснять видео с объектом и без него:

	/videos/with.mp4 && /videos/without.mp4
	
2. С помощью утилиты Free Video To JPEG Converter разобрать на изображения

	/extracted_images/with && /extracted_images/without/
	NB: Наибольшее количество файлов можно подсчитать как время (с) * 30 кадров/с
	
3. С помощью программы Fast Stone Viewer переименовать и изменить размер каждого изображения:

	Сервис - Пакетнгое преобразование - Переименование
	Переименовать в: with####

	Сервис - Пакетное преобразование - Добавить все. 
	В разделе доп. настроек выберем разрешение 800х600
	Сохранять пропорции = true
	Спрашивать при перезаписи = false
	Сохранить в папки 
	
		- preparing data\extracted_images\with_800 и 
		- preparing data\extracted_images\without_800
	
4. Запускаем встроенную утилиту
		opencv_annotation.exe --annotations=good.dat --images=./extracted_images/with_800