#include <curl/curl.h>
#include <string>
#include <iostream>
#include "ai.hpp"
#include <fmt/format.h>

// 响应数据回调函数
static size_t
WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
  size_t totalSize = size * nmemb;
  output->append((char *)contents, totalSize);
  return totalSize;
}

std::string TodoAI::getSummary(const std::string &todoList)
{
  CURL *curl;
  CURLcode res;
  std::string response;

  // 从环境变量获取 API Key
  const char *apiKey = std::getenv("DEEPSEEK_API_KEY");
  if (!apiKey)
  {
    std::cerr << "Error: DEEPSEEK_API_KEY environment variable not set"
              << std::endl;
    return "";
  }

  // JSON 请求体
  std::string jsonData = fmt::format(
      R"({{
        "model": "deepseek-chat",
        "messages": [
            {{"role": "system", "content": "你是一位专业秘书，接下来的会话请生成纯文本而非markdown富文本"}},
            {{"role": "user", "content": "请总结以下TodoList: {}, 请生成纯文本而非markdown富文本"}}
        ],
        "stream": false
    }})",
      todoList
  );
  // 初始化 curl
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();

  if (curl)
  {
    struct curl_slist *headers = NULL;

    // 添加请求头
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // 构建 Authorization 头
    std::string authHeader = "Authorization: Bearer " + std::string(apiKey);
    headers = curl_slist_append(headers, authHeader.c_str());

    // 设置请求参数
    curl_easy_setopt(
        curl, CURLOPT_URL, "https://api.deepseek.com/chat/completions"
    );
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonData.length());

    // 设置响应回调
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // 设置超时
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    // 执行请求
    res = curl_easy_perform(curl);

    // 检查结果
    if (res != CURLE_OK)
    {
      std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
    }
    else
    {
      long httpCode = 0;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

      std::cout << "HTTP Status: " << httpCode << std::endl;
      std::cout << "Response: " << response << std::endl;
    }

    // 清理
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  return "";
}

// int main()
// {
//   std::string todoList =
//       "1 Todo Title: apply TODO for your daily work !, Start Date: 2026/1/28, "
//       "Status: underway; 1.1 Todo Title: show tree structure, Start Date: "
//       "2026/1/28, Status: underway; 1.2 Todo Title: todo :: a better GUI, "
//       "Start Date: 2026/1/28, Status: underway; 1.2.1 Todo Title: a better UI "
//       "experience, Start Date: 2026/2/11, Status: underway; 1.3 Todo Title: "
//       "todo :: add arrow keys support, Start Date: 2026/1/28, Status: closed; "
//       "1.4 Todo Title: 给fzf添加ignore, Start Date: 2026/1/30, Status: "
//       "underway; 2 Todo Title: top hits doc, Start Date: 2026/1/28, Status: "
//       "closed; 2.1 Todo Title: AI script for tophits , Start Date: 2026/1/28, "
//       "Status: underway; 3 Todo Title: SpaceWar, Start Date: 2026/1/28, "
//       "Status: underway; 3.1 Todo Title: draw a dialog box with glfw, Start "
//       "Date: 2026/1/28, Status: underway; 4 Todo Title: 配置lazyvim ！！, "
//       "Start Date: 2026/1/28, Status: closed; 5 Todo Title: "
//       "尝试新手机做主力机, Start Date: 2026/1/28, Status: closed; 6 Todo "
//       "Title: 安卓同层渲染, Start Date: 2026/1/28, Status: underway; 6.1 Todo "
//       "Title: 找到AppBrand的同层渲染实例并debug一下, Start Date: 2026/1/30, "
//       "Status: underway; 7 Todo Title: 配置citron编译, Start Date: 2026/1/28, "
//       "Status: underway; 8 Todo Title: PainterEngine 踩坑学习, Start Date: "
//       "2026/1/30, Status: underway; 9 Todo Title: zig blog: "
//       "构建自己的博客系统, Start Date: 2026/1/30, Status: underway; 10 Todo "
//       "Title: 鸿蒙本地搜最常使用实验放量, Start Date: 2026/2/2, Status: "
//       "underway; 11 Todo Title: zinux游戏配置, Start Date: 2026/2/2, Status: "
//       "closed;";
//   TodoAI::getSummary(todoList);
//   return 0;
// }
