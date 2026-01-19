#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>

namespace mytool {
	class LOGGER {
	private:
		std::ofstream logger;

		bool create_path(const std::string& path) {
			try {
				if (!std::filesystem::exists(path)) {
					return std::filesystem::create_directories(path);
				}
				return true;
			}
			catch (const std::exception& e) {
				std::cerr << "创建文件夹时出错: " << e.what() << std::endl;
				system("pause");
				return false;
			}
		}

	public:
		LOGGER(const std::string& path, const std::string& file) { open(path, file); }
		~LOGGER() {
			if (logger.is_open()) {
				logger.close();
			}
		}

		template<typename... Args>
		bool log(Args... args) {
			if (logger.is_open()) {
				(logger << ... << std::forward<Args>(args));
				logger << std::endl;
				logger.flush();
				return true;
			}
			return false;
		}

		bool open(const std::string path, const std::string& file) {
			create_path(path);
			if (!logger.is_open()) {
				logger.open(path + '/' + file, std::ios::out | std::ios::app);
				return true;
			}
			return false;
		}
	};
}