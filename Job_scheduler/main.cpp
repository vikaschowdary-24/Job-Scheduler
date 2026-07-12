#include "Job.h"
#include "ExecutionEngine.h"

#include <vector>
#include <memory>
#include <iostream>

int main()
{
    std::vector<std::shared_ptr<Job>> jobs = {

        // Root Job
        std::make_shared<Job>(
            "UploadVideo",
            2,
            5,
            10,
            std::vector<std::string>{}
        ),

        // Validation
        std::make_shared<Job>(
            "ValidateVideo",
            1,
            8,
            20,
            std::vector<std::string>{"UploadVideo"}
        ),

        // Parallel processing
        std::make_shared<Job>(
            "ExtractAudio",
            2,
            12,
            15,
            std::vector<std::string>{"ValidateVideo"}
        ),

        std::make_shared<Job>(
            "ExtractFrames",
            3,
            15,
            15,
            std::vector<std::string>{"ValidateVideo"}
        ),

        std::make_shared<Job>(
            "ExtractMetadata",
            1,
            10,
            12,
            std::vector<std::string>{"ValidateVideo"}
        ),

        // Audio processing
        std::make_shared<Job>(
            "NoiseReduction",
            3,
            18,
            18,
            std::vector<std::string>{"ExtractAudio"}
        ),

        std::make_shared<Job>(
            "SpeechToText",
            4,
            24,
            22,
            std::vector<std::string>{"NoiseReduction"}
        ),

        // Video processing
        std::make_shared<Job>(
            "FrameEnhancement",
            4,
            22,
            20,
            std::vector<std::string>{"ExtractFrames"}
        ),

        std::make_shared<Job>(
            "ObjectDetection",
            5,
            28,
            25,
            std::vector<std::string>{"FrameEnhancement"}
        ),

        // Merge point
        std::make_shared<Job>(
            "GenerateCaptions",
            2,
            32,
            30,
            std::vector<std::string>{
                "SpeechToText",
                "ObjectDetection",
                "ExtractMetadata"
            }
        ),

        std::make_shared<Job>(
            "RenderFinalVideo",
            5,
            40,
            35,
            std::vector<std::string>{"GenerateCaptions"}
        ),

        std::make_shared<Job>(
            "UploadToCDN",
            2,
            45,
            40,
            std::vector<std::string>{"RenderFinalVideo"}
        ),

        std::make_shared<Job>(
            "NotifyUsers",
            1,
            48,
            50,
            std::vector<std::string>{"UploadToCDN"}
        )
    };

    std::cout << "Initializing Execution Engine with "
              << jobs.size() << " jobs..." << std::endl;

    ExecutionEngine engine(2, jobs);

    std::cout << "Starting execution workflow..." << std::endl;

    engine.execute();

    std::cout << "Workflow execution completed." << std::endl;

    return 0;
}