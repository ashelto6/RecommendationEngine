add_subdirectory(data)

set(PROJECT_SOURCES
        RecommendationEngine.cpp
        data/readCSVData.cpp
)

add_executable(RecommendationEngine ${PROJECT_SOURCES})