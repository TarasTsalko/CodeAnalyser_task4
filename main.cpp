#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"


int main(int argc, char *argv[]) {

    analyser::cmd::ProgramOptions options;

    // распарсите входные параметры
    if( !options.Parse( argc, argv ) )
        return 0;

    namespace metric_impl      = analyser::metric::metric_impl;
    namespace accamulator_impl = analyser::metric_accumulator::metric_accumulator_impl;
    namespace accumulator_interface = analyser::metric_accumulator;

    const std::vector<std::string>& inFiles = options.GetFiles();

    // analyser::metric::MetricExtractor metric_extractor;
    // зарегистрируйте метрики в metric_extractor
    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric( std::make_unique<metric_impl::CodeLinesCountMetric>() );
    metric_extractor.RegisterMetric( std::make_unique<metric_impl::CountParametersMetric>() );
    metric_extractor.RegisterMetric( std::make_unique<metric_impl::CyclomaticComplexityMetric>() );
    
    // запустите analyser::AnalyseFunctions
    auto metricsToFuncs = analyser::AnalyseFunctions( inFiles, metric_extractor );
    
    // зарегистрируйте аккумуляторы метрик в accumulator
    analyser::metric_accumulator::MetricsAccumulator accumulator;
    accumulator.RegisterAccumulator( "CodeLinesCountMetric", std::make_unique<accamulator_impl::SumAverageAccumulator>() );
    accumulator.RegisterAccumulator( "CyclomaticComplexityMetric", std::make_unique<accamulator_impl::SumAverageAccumulator>() );
    accumulator.RegisterAccumulator( "CountParametersMetric", std::make_unique<accamulator_impl::AverageAccumulator>() );
    
    std::println( "========Разбиение по файлам===========" );
    auto splittedByFiles = analyser::SplitByFiles( metricsToFuncs );
    analyser::PrintResultAnalyseSplittedByGroup( splittedByFiles, accumulator, "file" );
   
    std::println();
    std::println( "========Разбиение по классам===========" );
    auto splittedByClasses = analyser::SplitByClasses( metricsToFuncs );
    analyser::PrintResultAnalyseSplittedByGroup( splittedByClasses, accumulator, "class" );
   
    // запустите analyser::AccumulateFunctionAnalysis для всех результатов метрик
    // выведете результаты на консоль

    return 0;
}
