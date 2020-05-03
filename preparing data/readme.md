### Инструкция по подготовке изображений для обучения каскада

1. Заснять видео с объектом и без него:

	    /videos/with.mp4 && /videos/without.mp4
	
2. С помощью утилиты **Free Video To JPEG Converter** разобрать на изображения

	    /extracted_images/with && /extracted_images/without/
	    
	    NB: Наибольшее количество файлов можно подсчитать как время (с) * 30 кадров/с
	
3. С помощью программы **Fast Stone Viewer** переименовать и изменить размер каждого изображения:

        Сервис - Пакетнгое преобразование - Переименование
        Переименовать в: with####
    
        Сервис - Пакетное преобразование - Добавить все. 
        В разделе доп. настроек выберем разрешение 800х600
        Сохранять пропорции = true
        Спрашивать при перезаписи = false
        
        Сохранить в папки 
	
		- preparing data\extracted_images\with_800 и 
		- preparing data\extracted_images\without_800
	
4. Размечаем изображения с помощью встроенной утилиты **opencv_annotation.exe**

    > opencv_annotation.exe --annotations=good.dat --images=./extracted_images/with_800
          
    0. Процесс разметки может оказаться очень и очень долгим.
    
        Можно его прервать нажатием клавиши Escape, а уже обработанные файлы перенести в папку *already_processed*
        с помощью python-скрипта **move_files.py**.
        
        При этом потребуется поправить пути в файле good.dat. Поможет скрипт **fix_pathes.py**
      
5. Создаём вектор

        opencv_createsamples.exe -info good_fixed.dat -vec samples.vec -num 1372 -w 42 -h 48 -show
        
        Тестова команда
        opencv_createsamples.exe -info good_fixed.dat -vec samples.vec -num 10 -w 100 -h 100 -show
        
    > Чем больше размер, тем дольше обучение. Также важно учесто пропорции объекта.
    > Соотношение высоты к ширине для моего **Ждуна** составляет 11 к 10
        
6. Обучаем каскад командой

        opencv_traincascade.exe -data haar_results_v2 -vec samples.vec -bg bad.dat -numStages 16 -numThreads 10 -w 42 -h 48 -precalcValBufSize 2048 -precalcIdxBufSize 2048 -minhitrate 0.995 -maxFalseAlarmRate 0.5 -numPos 1200 -numNeg 2955 -mode ALL
        opencv_traincascade.exe -data haar_results_v2 -vec samples.vec -bg bad.dat -numStages 16 -numThreads 12 -w 42 -h 48 -numPos 1200 -numNeg 2955 -mode ALL
        opencv_traincascade.exe -data haar_results_v2 -vec samples.vec -bg bad.dat -numStages 16 -numThreads 12 -w 42 -h 48 -numPos 200 -numNeg 600 -mode ALL

        opencv_traincascade.exe -data haar_results2 -vec samples2.vec -bg bad.dat -numStages 16 
        -numThreads 12 -precalcValBufSize 2048 -precalcIdxBufSize 2048 -minhitrate 0.990 
        -maxFalseAlarmRate 0.5 -numPos 200 -numNeg 499 -mode ALL
		
### Проблемы с Git:

1. Отменитиь git add .

		git reset
		git reset myfile.cpp
		
		
2. Отменить последний коммит:

		git reset HEAD~
		
3. Показать игнорируемые файлы:

		git status --ignored
		
		Также можно пользоваться конвейерами:
		
		git status | grep "mp4"
		
4. Удалить файлы из отслеживаемых

		git rm --cached "*.mp4"
		git rm --cached "*.jpg"
		
		https://stackoverflow.com/questions/3833561/why-doesnt-git-ignore-my-specified-file
