### Подготовка данных

#### Ахтунг!

#### Маркером №1 назовём **чёрный круг** (два верхних), маркером №2 - белый круг (два нижних)

1. Размечаем изображения с помощью встроенной утилиты **opencv_annotation.exe**

		//opencv_annotation.exe --annotations=good_1.dat --images=./extracted_images/with_original
		
		opencv_annotation.exe --annotations=good_2.dat --images=./extracted_images/with_original
		
		
2. Создаём вектор программой **opencv_createsamples.exe**:

    - **haar_navigation_m1_v1:**
     
            opencv_createsamples.exe -info good_1_fixed.dat -vec samples_navigation_m1_v1.vec -num 600 -w 24 -h 24 -show

    - **haar_navigation_m1_v2:**
     
            opencv_createsamples.exe -info good_1_fixed.dat -vec samples_navigation_m1_v2.vec -num 1274 -w 24 -h 24 -show

    - **haar_navigation_m2_v1:**
     
            opencv_createsamples.exe -info good_2_fixed.dat -vec samples_navigation_m2_v1.vec -num 600 -w 24 -h 24 -show

3. Обучаем каскад командой

    - **haar_navigation_m1_v1:** 
    
            opencv_traincascade.exe -data haar_navigation_m1_v1 -vec samples_navigation_m1_v1.vec -bg bad_navigation.dat -numStages 16 -numThreads 12 -w 24 -h 24 -numPos 500 -numNeg 2955 -mode ALL

    - **haar_navigation_m1_v2:** 
    
            opencv_traincascade.exe -data haar_navigation_m1_v2 -vec samples_navigation_m1_v2.vec -bg bad_navigation.dat -numStages 16 -numThreads 12 -w 24 -h 24 -numPos 1100 -numNeg 2955 -mode ALL

    - **haar_navigation_m2_v1:** 
    
            opencv_traincascade.exe -data haar_navigation_m2_v1 -vec samples_navigation_m2_v1.vec -bg bad_navigation.dat -numStages 16 -numThreads 12 -w 24 -h 24 -numPos 500 -numNeg 2955 -mode ALL
