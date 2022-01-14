default:
	g++ -std=c++17 -pthread RecommendationEngine.cpp data/readCSVData.cpp -o recommendations
	./recommendations
