 ====================================
 .\createsamples.exe -info E:\University\10sem\nirs\haar_cascade\waiter\Good.dat -vec samples.vec -w 20 -h 20

 .\createsamples.exe 
 -info E:\University\10sem\nirs\haar_cascade\waiter\Good.dat 
 -vec samples.vec 
 -w 20 -h 20 
 -num 5
 -show
 
 opencv_createsamples.exe -info good.dat -vec samples2.vec -show
 
 ====================================
 Из статьи:
	opencv_traincascade.exe -data haarcascade -vec samples.vec -bg Bad.dat -numStages 16 -minhitrate 0.990 -maxFalseAlarmRate 0.5 -numPos 400 -numNeg 500 -w 20 -h 20 -mode ALL -precalcValBufSize 256 -precalcIdxBufSize 256
 Моё:
	opencv_traincascade.exe -data haar_results -vec samples.vec -bg Bad.dat -numStages 16 -minhitrate 0.990 -maxFalseAlarmRate 0.5 -numPos 5 -numNeg 26 -w 20 -h 20 -mode ALL -precalcValBufSize 256 -precalcIdxBufSize 256
	
	opencv_traincascade.exe -data haar_results2 -vec samples2.vec -bg bad.dat -numStages 16 -minhitrate 0.990 -maxFalseAlarmRate 0.5 -numPos 305 -numNeg 499 -mode ALL
	
		NB: количество положительных изображений должно быть меньше, чем кол-во, использованное для созания vec-файла.
		Поскольку алгоритм в процесе обучения не распознаёт все изображения, некоторые пропускает. И на пропущенных учится дальше
	
	opencv_traincascade.exe -data haar_results2 -vec samples2.vec -bg bad.dat -numStages 16 -minhitrate 0.990 -maxFalseAlarmRate 0.5 -numPos 200 -numNeg 499 -mode ALL
	opencv_traincascade.exe -data haar_results2 -vec samples2.vec -bg bad.dat -numStages 16 -numThreads 12 -precalcValBufSize 2048 -precalcIdxBufSize 2048 -minhitrate 0.990 -maxFalseAlarmRate 0.5 -numPos 200 -numNeg 499 -mode ALL
	
	Training until now has taken 0 days 0 hours 2 minutes 29 seconds.

	===== TRAINING 12-stage =====
	<BEGIN
	POS count : consumed   200 : 200
	NEG count : acceptanceRatio    0 : 0
	Required leaf false alarm rate achieved. Branch training terminated.
 
 
 ====================================
 opencv_annotation.exe -a=anno.txt -i=../../waiter/Good -m=640
 
 opencv_annotation.exe -a=good.dat -i=with_resized
 
 
 ====================================
 opencv_visualisation.exe -i=217586.bmp -m=haar_results/cascade.xml -d=visual_results/
 
 
 
 
 
 
 
 