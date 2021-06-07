// dyn-contract-loader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "nlohmann/json.hpp"
#include <curl\curl.h>


struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


int main()
{
    using json = nlohmann::json;

    
    std::string strCommand = "{ \"id\": 0, \"method\" : \"sendtoaddress\", ";
    strCommand = strCommand + "\"params\" : [ \"dy1qxj4awv48k7nelvwwserdl9wha2mfg6w3wy05fc\", 0.1, \"\",  \"\", null, null, null, null, null, null, true ], ";
    strCommand = strCommand + "\"nft_command\" : \"0045d93cc8ed347c2e42321fb869ec0cd6704380eb39efcf6b8202a15efb4ab63a\"  }";
    

    json j = strCommand;

    std::string jData = j.dump();

    struct MemoryStruct chunk;

    chunk.memory = (char*)malloc(1);
    chunk.size = 0;

    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    std::string transactionHex;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://10.1.0.114:6433");
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_USERNAME, "user");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "123456");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strCommand.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else {
            json result = json::parse(chunk.memory);
            printf("%s\n", result.dump().c_str());
            transactionHex = result["result"]["txid"];
        }

    }


    //{"name":"first one","color":"blue"}
    //7B226E616D65223A226669727374206F6E65222C22636F6C6F72223A22626C7565227D
    //64 bit count 0000000000000100 = 256 decimal

    strCommand = "{ \"id\": 0, \"method\" : \"submitnft\", ";
    strCommand = strCommand + "\"params\" : [ \"add-class\", \"00237B226E616D65223A226669727374206F6E65222C22636F6C6F72223A22626C7565227D0000000000000100\", \"dy1qxj4awv48k7nelvwwserdl9wha2mfg6w3wy05fc\", \"" + transactionHex + "\", \"\"  ] ";
    strCommand = strCommand + "  }";

    chunk.size = 0;

    std::string nftID;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://10.1.0.114:6433");
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_USERNAME, "user");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "123456");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strCommand.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else {
            json result = json::parse(chunk.memory);
            printf("%s\n", result.dump().c_str());
            nftID = result["result"];
        }

    }


    std::vector<unsigned char> asset_data = { '0','1','2' };
    std::string asset_data_hex = "484950";

    std::string nft_command;
    nft_command += "00237B226E616D65223A226669727374206F6E65222C22636F6C6F72223A22626C7565227D";   //meta junk
    nft_command += "000003" + asset_data_hex;
    nft_command += "0000000000000001";


    strCommand = "{ \"id\": 0, \"method\" : \"sendtoaddress\", ";
    strCommand = strCommand + "\"params\" : [ \"dy1qxj4awv48k7nelvwwserdl9wha2mfg6w3wy05fc\", 0.1, \"\",  \"\", null, null, null, null, null, null, true ], ";
    strCommand = strCommand + "\"nft_command\" : \"012f9f136134b29f0953c2b37495b4a86ab3d8e2bd29c8ae510ec22f8236dcad39\"  }";


    chunk.size = 0;


    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://10.1.0.114:6433");
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_USERNAME, "user");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "123456");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strCommand.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else {
            json result = json::parse(chunk.memory);
            printf("%s\n", result.dump().c_str());
            transactionHex = result["result"]["txid"];;
        }

    }

    nft_command = "00237B226E616D65223A226669727374206F6E65222C22636F6C6F72223A22626C7565227D";   //meta junk
    nft_command += "000003" + asset_data_hex;
    nft_command += "0000000000000001";

    strCommand = "{ \"id\": 0, \"method\" : \"submitnft\", ";
    strCommand = strCommand + "\"params\" : [ \"add-asset\", \""+ nft_command + "\", \"dy1qxj4awv48k7nelvwwserdl9wha2mfg6w3wy05fc\", \"" + transactionHex + "\", \"" + nftID + "\"  ] ";
    strCommand = strCommand + "  }";

    chunk.size = 0;


    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://10.1.0.114:6433");
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_USERNAME, "user");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "123456");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strCommand.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else {
            json result = json::parse(chunk.memory);
            printf("%s\n", result.dump().c_str());
            nftID = result["result"];
        }

    }


    curl_easy_cleanup(curl);
    free(chunk.memory);
    curl_global_cleanup();  
}

