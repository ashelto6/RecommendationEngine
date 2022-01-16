default:
	g++ -std=c++17 -pthread RecommendationEngine.cpp data/readCSVData.cpp -o recommendations
	./recommendations data/profileRatings_sm.csv data/youtube_trending_videos_data_sm.csv
