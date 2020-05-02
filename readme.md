#### Пример обучения каскада Хаара

- Пример работы каскада - в папке haar_3_4_6
- Подготовка данных для обучения - папка preparing data

	Результат работы классификатора лежит в папке **preparing_data/haar_results2**

	- Использовано 200 позитивных и 499 негативных примеров разрешением 640х480 пикселей.

	- Позитивные примеры (**305 шт.**) размечены вручную с помощью утилиты

	> opencv_annotation.exe -a=good.dat -i=with_resized

	- Размеры примитива по умолчанию: 24 х 24 пикселя 
	> opencv_createsamples.exe -info good.dat -vec samples2.vec -show
	>
	> opencv_traincascade.exe -data haar_results2 -vec samples2.vec -bg bad.dat -numStages 16 -numThreads 12 -precalcValBufSize 2048 -precalcIdxBufSize 2048 -minhitrate 0.990 -maxFalseAlarmRate 0.5 -numPos 200 -numNeg 499 -mode ALL

		===== TRAINING 12-stage =====
		<BEGIN
		POS count : consumed   200 : 200
		NEG count : acceptanceRatio    0 : 0
		Required leaf false alarm rate achieved. Branch training terminated.