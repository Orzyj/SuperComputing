#pragma once
#include <string>

/**
 * @namespace Config
 * @brief Global configuration constants for the application.
 */
namespace Config {

    /**
     * @namespace Config::Files
     * @brief File names for data export and reporting.
     */
    namespace Files {
        /** @brief CSV filename for Bubble Sort results. */
        constexpr const char* BUBBLE_SORT_CSV = "wyniki_sortowania_bubble.csv";

        /** @brief CSV filename for Merge Sort results. */
        constexpr const char* MERGE_SORT_CSV = "wyniki_sortowania_merge.csv";

        /** @brief CSV filename for Quick Sort results. */
        constexpr const char* QUICK_SORT_CSV = "wyniki_sortowania_quick.csv";

        /** @brief Filename for the generated performance report. */
        constexpr const char* REPORT_PDF = "raport.html";
    }

    /**
     * @namespace Config::Images
     * @brief File names for generated chart images.
     */
    namespace Images {
        /** @brief PNG filename for the Bubble Sort performance chart. */
        constexpr const char* BUBBLE_CHART = "wykres_bubble.png";

        /** @brief PNG filename for the Merge Sort performance chart. */
        constexpr const char* MERGE_CHART = "wykres_merge.png";

        /** @brief PNG filename for the Quick Sort performance chart. */
        constexpr const char* QUICK_CHART = "wykres_quick.png";
    }

    /**
     * @namespace Config::SortingAlgorithms
     * @brief Display names for the sorting algorithms.
     */
    namespace SortingAlgorithms {
        /** @brief Display name for Bubble Sort. */
        constexpr const char* BUBBLE = "Bubble Sort";

        /** @brief Display name for Merge Sort. */
        constexpr const char* MERGE = "Merge Sort";

        /** @brief Display name for Quick Sort. */
        constexpr const char* QUICK = "Quick Sort";
    }
}