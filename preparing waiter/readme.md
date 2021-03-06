### 1. Демонстрация работы каскада Хаара

![](./first_success.png)

***

### 2. Сравнение двух каскадов: haar_results_v2 vs haar_results_v6:

![](./research_1.png)

Второй каскад (слева) имел много больше False Positive срабатываний

***

### 3. Сводная таблица параметров каскадов

Название |      Позитивных |      Негативных |      Width |         Height   | unique features | Time |
--------------- | ------------- | ------------- | ------------- | ------------- | ------------- | -----------
haar_results_v6 |   1500    |      2955      |      22       |      24       |    219 180    |  1h 23m 03s
haar_results_v5 |   1200    |      2955      |      22       |      24       |    219 180    |  1h 04m 31s
haar_results_v4 |   200     |       600      |      22       |      24       |      219 180  |  0h 02m 02s
--------------- |   ---     |       ---      |      --       |      --       |    --- ---    |  -- -- --
haar_results_v3 |   200     |       600      |      33       |      36       |    1 121 307  |  1h 22m 21s
haar_results_v2 |   200     |       600      |      42       |      48       |    3 239 144  |  1h 50m 52s
haar_results_v7 |   1500     |     2955      |      42       |      48       |    3 239 144  |  not finished

##### Выводы:

1. Как видно из таблицы, колчество уникальных свойств зависит от размера окна сканирования

### 4. Инструкция по подготовке изображений и обучению каскада

1. Заснять видео с объектом и без него:

	    /videos/with.mp4 
	    &&
	    /videos/without.mp4
	
2. С помощью утилиты **Free Video To JPEG Converter** разобрать на изображения и поместить в соотв. каталоги

	    /extracted_images/with 
	    &&
	    /extracted_images/without/
	    
    > NB: Наибольшее количество файлов можно подсчитать как время (с) * 30 кадров/с
	
3. С помощью программы **Fast Stone Viewer** переименовать и изменить размер каждого изображения:

    - Сервис - Пакетное преобразование - Переименование.
      Переименовать в:
       
            with#### и without####
    
    - Сервис - Пакетное преобразование - Добавить все. 
        + В разделе доп. настроек выберем разрешение 800 х 600
        + Сохранять пропорции = true
        + Спрашивать при перезаписи = false
        
    - Сохранить в папки 
	
            - preparing data\extracted_images\with_800 и 
            - preparing data\extracted_images\without_800
	
4. Размечаем изображения с помощью встроенной утилиты **opencv_annotation.exe**

        opencv_annotation.exe --annotations=good.dat --images=./extracted_images/with_800
          
    > Процесс разметки может оказаться очень и очень долгим.
    >                                                                                                                                                                                     
    > Можно его прервать нажатием клавиши Escape, а уже обработанные файлы перенести в папку **already_processed**
    с помощью python-скрипта **move_files.py**.
    При этом потребуется поправить пути в файле good.dat. Поможет скрипт **fix_pathes.py**
      
5. Создаём вектор программой **opencv_createsamples.exe**:

    - **haar_results_v2:**
     
            opencv_createsamples.exe -info good_fixed.dat -vec samples.vec -num 1372 -w 42 -h 48 -show
    
    - **haar_results_v3:**
     
            opencv_createsamples.exe -info good_fixed.dat -vec samples_33_36.vec -num 1372 -w 33 -h 36 -show
            
    - **haar_results_v4:**
     
            opencv_createsamples.exe -info good_fixed.dat -vec samples_22_24.vec -num 1372 -w 22 -h 24 -show
        
    - **haar_results_v5:**
     
            opencv_createsamples.exe -info good_fixed.dat -vec samples_22_24.vec -num 1372 -w 22 -h 24 -show
            
    - **haar_results_v6:**
     
            opencv_createsamples.exe -info good_fixed.dat -vec samples_22_24_v6.vec -num 1500 -w 22 -h 24 -show

    - **haar_results_v7:**
     
            opencv_createsamples.exe -info good_fixed.dat -vec samples_42_48_v7.vec -num 1200 -w 42 -h 48 -show

    - **haar_results_v8:**
     
            opencv_createsamples.exe -info good_fixed.dat -vec samples_33_36_v8.vec -num 1500 -w 33 -h 36 -show
            
    - **Тестовая команда**
    
            opencv_createsamples.exe -info good_fixed.dat -vec samples.vec -num 10 -w 100 -h 100 -show
        
    > Чем больше размер, тем дольше обучение. Также важно учесто пропорции объекта.
      Соотношение высоты к ширине для моего **Ждуна** составляет 11 к 10
        
6. Обучаем каскад командой

    > NB: количество положительных изображений должно быть меньше, чем кол-во, использованное для созания vec-файла.
	  Поскольку алгоритм в процесе обучения распознаёт не все изображения, некоторые пропускает. И на пропущенных учится дальше

    - **haar_results_v1:**
    
            opencv_traincascade.exe -data haar_results1 -vec samples2.vec -bg bad.dat -numStages 16 
            -numThreads 12 -precalcValBufSize 2048 -precalcIdxBufSize 2048 -minhitrate 0.990 
            -maxFalseAlarmRate 0.5 -numPos 200 -numNeg 499 -mode ALL

    - **haar_results_v2:** 
    
            opencv_traincascade.exe -data haar_results_v2 -vec samples.vec -bg bad.dat -numStages 16 
            -numThreads 12 -w 42 -h 48 -numPos 200 -numNeg 600 -mode ALL
    
    - **haar_results_v3:** 
    
            opencv_traincascade.exe -data haar_results_v3 -vec samples_33_36.vec -bg bad.dat -numStages 16 
			-numThreads 12 -w 33 -h 36 -numPos 200 -numNeg 600 -mode ALL

    - **haar_results_v4:** 
    
            opencv_traincascade.exe -data haar_results_v4 -vec samples_22_24.vec -bg bad.dat -numStages 16 -numThreads 12 -w 22 -h 24 -numPos 200 -numNeg 600 -mode ALL
            
    - **haar_results_v5:** 
    
            opencv_traincascade.exe -data haar_results_v5 -vec samples_22_24.vec -bg bad.dat -numStages 16 -numThreads 12 -w 22 -h 24 -numPos 1200 -numNeg 2955 -mode ALL

    - **haar_results_v6:** 
    
            opencv_traincascade.exe -data haar_results_v6 -vec samples_22_24_v6.vec -bg bad.dat -numStages 16 -numThreads 12 -w 22 -h 24 -numPos 1500 -numNeg 2955 -mode ALL

    - **haar_results_v7:** 
    
            opencv_traincascade.exe -data haar_results_v7 -vec samples_42_48_v7.vec -bg bad.dat -numStages 16 -numThreads 12 -w 42 -h 48 -numPos 1200 -numNeg 2955 -mode ALL

    - **haar_results_v8:** 
    
            opencv_traincascade.exe -data haar_results_v8 -vec samples_33_36_v8.vec -bg bad.dat -numStages 16 -numThreads 12 -w 33 -h 36 -numPos 1500 -numNeg 2955 -mode ALL


    - **То, что должно быть в идеале:**
    
            opencv_traincascade.exe -data haar_results_v2 -vec samples.vec -bg bad.dat -numStages 16 -numThreads 10
             -w 42 -h 48 -precalcValBufSize 2048 -precalcIdxBufSize 2048 -minhitrate 0.995 -maxFalseAlarmRate 0.5 
             -numPos 1200 -numNeg 2955 -mode ALL
            
            opencv_traincascade.exe -data haar_results_v2 -vec samples.vec -bg bad.dat -numStages 16 -numThreads 12 
            -w 42 -h 48 -numPos 1200 -numNeg 2955 -mode ALL
        
7. Визуализация работы обученного каскада

(работает странно)

		opencv_visualisation.exe -i=217586.bmp -m=haar_results/cascade.xml -d=visual_results/


***

### 5. Решение проблемы с ошибкой 
#### Bad argument (Can not get new positive sample. The most possible reason is insufficient count of samples in given vec-file

https://answers.opencv.org/question/4368/traincascade-error-bad-argument-can-not-get-new-positive-sample-the-most-possible-reason-is-insufficient-count-of-samples-in-given-vec-file/

Формула для расчёта необходимого количества изображений:

> vec-file has to contain >= (numPose + (numStages-1) * (1 - minHitRate) * numPose) + S

1. Увеличить разницу в количестве позитивных изображений, передаваемых каскаду для обучения,
и общего числа изображений, использованных для создания vec-файла. К примеру, при создании вектора
было указано 600 изображений, а traincascade передать аргумент -numPos=500

2. Уменьшить minHitRate

3. Таки увеличить количество позитивных изображений (потребуется заново создать vec-файл)
		
***

### 6. Проблемы с Git:

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
