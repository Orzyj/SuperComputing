#pragma once
#include <string>

namespace Config {
    namespace Files {
        constexpr const char* BUBBLE_SORT_CSV = "wyniki_sortowania_bubble.csv";
        constexpr const char* MERGE_SORT_CSV = "wyniki_sortowania_merge.csv";
        constexpr const char* QUICK_SORT_CSV = "wyniki_sortowania_quick.csv";
		constexpr const char* REPORT_PDF = "raport.html";
    }
    namespace Images {
        constexpr const char* BUBBLE_CHART = "wykres_bubble.png";
        constexpr const char* MERGE_CHART = "wykres_merge.png";
		constexpr const char* QUICK_CHART = "wykres_quick.png";
    }

    namespace SortingAlgorithms {
		constexpr const char* BUBBLE = "Bubble Sort";
		constexpr const char* MERGE = "Merge Sort";
		constexpr const char* QUICK = "Quick Sort";
    }
}
