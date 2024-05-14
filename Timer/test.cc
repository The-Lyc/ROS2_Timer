#include <iostream>
#include <fstream>
#include <chrono>

int main() {
    std::ofstream outFile("output.txt");

    if (!outFile) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // 测量开始时间
    auto start = std::chrono::steady_clock::now();

    // 向文件写入 1e4 个 int
    for (int i = 0; i < 1e6; ++i) {
        outFile << i << std::endl;
    }

    // 测量结束时间
    auto end = std::chrono::steady_clock::now();

    // 计算时间间隔
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // 输出结果
    std::cout << "Time taken to write 1e4 integers to file: " << duration.count() << " milliseconds" << std::endl;

    // 关闭文件流
    outFile.close();

    return 0;
}
