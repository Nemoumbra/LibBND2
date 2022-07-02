#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <filesystem>
#include <fstream>
#include <type_traits>
#include "BND.h"

namespace fs = std::filesystem;

void log(const std::string& str, std::ostream& stream, const std::string& sep) {
	stream << str << sep;
}

uint32_t count_dirs(const BND& bnd) { // shouldn't this thing be O(1)?
	//return bnd.files.size() - bnd.count_files();
	return 0;
}
bool contains(const BND& bnd, const std::string& filesystem_object_path_inside_BND) { // assuming BND is loaded
	// returns true <=> there is a directory with the given path
	for (int i = 0; i < bnd.files.size(); ++i) {
		if (bnd.files[i].name == filesystem_object_path_inside_BND) {
			return true;
		}
	}
	return false;
}
int get_id_by_path_inside_BND(const BND& bnd, const std::string& filesystem_object_path_inside_BND) {
	for (int i = 0; i < bnd.files.size(); ++i) {
		if (bnd.files[i].name == filesystem_object_path_inside_BND) {
			return i;
		}
	}
	return -1;
}




bool test_small_bnd(const fs::path& path) { // It's ok to discard the return value from this function
	std::ofstream output;

	std::cout << "Starting small tests\n";
	std::cout << "Print logs to screen?\n";
	bool confirmation;
	std::cin >> confirmation;
	if (std::cin.fail()) {
		std::cout << "Wrong input, quitting.\n";
		exit(1);
	}
	bool use_cout;
	use_cout = confirmation;
	if (!use_cout) {
		fs::path source;
		std::cout << "Enter the path to log the small tests\n";
		std::cin >> source;
		output.open(source);
		if (!output) {
			std::cout << "Can't use" << source << "\n";
			std::cout << "Standard output will be used\n";
			use_cout = true;
		}
	}

	BND small_bnd;
	try {
		if (!small_bnd.load((path / "small.bnd").string())) {
			std::cout << "small_bnd.load(...) claims the file was not loaded due to some error\n";
			std::cout << "Aborting small tests\n";
			return false;
		}
		// Place breakpoint here
		//log()
		log("small_bnd.count_files() returns", use_cout ? std::cout : output, " ");
		log(std::to_string(small_bnd.count_files()), use_cout ? std::cout : output, "\n\n");

		log("small_bnd.count_entries() returns", use_cout ? std::cout : output, " ");
		log(std::to_string(small_bnd.count_entries()), use_cout ? std::cout : output, "\n\n");
		log("Calling small_bnd.list_all_files()...", use_cout ? std::cout : output, "\n\n");

		small_bnd.list_all_files();
		std::cout << "\n";

		log("Trying to iterate over files...", use_cout ? std::cout : output, "\n");

		int k = 0;
		for (const auto& file : small_bnd.files) {
			++k;
			log("Filesystem object number", use_cout ? std::cout : output, " ");
			log(std::to_string(k), use_cout ? std::cout : output, "\n");
			int level = static_cast<int> (file.level);
			if (level < 0) {
				log("Object is a file entry", use_cout ? std::cout : output, "\n");
				log("File's level:", use_cout ? std::cout : output, " ");
				log(std::to_string(level), use_cout ? std::cout : output, "\n");
				log("File's name:", use_cout ? std::cout : output, " ");
				log(file.name, use_cout ? std::cout : output, "\n");
			}
			else {
				log("Object is a directory entry", use_cout ? std::cout : output, "\n");
				log("Directory's level:", use_cout ? std::cout : output, " ");
				log(std::to_string(level), use_cout ? std::cout : output, "\n");
				log("Directory's name:", use_cout ? std::cout : output, " ");
				log(file.name, use_cout ? std::cout : output, "\n");
			}
		}
		log("\n", use_cout ? std::cout : output, "");
		log("Calling small_bnd.list_sorted_via_offset()", use_cout ? std::cout : output, "\n");
		small_bnd.list_sorted_via_offset();


	}
	catch (...) {
		std::cout << "Error occured, please check library's report!";
		return false;
	}
	log("Extract tests:", use_cout ? std::cout : output, "\n");


	log("Small tests finished successfully!", use_cout ? std::cout : output, "\n");
	return true;
}


int main() {
	std::cout << "libBND2 test program\n";
	std::cout << "Please enter the path to the directory with the test files\n";
	fs::path tests_dir;
	std::cin >> tests_dir;
	
	try {
		if (!fs::is_directory(tests_dir)) {
			std::cout << "Directory " << tests_dir << " does not exist\n";
			std::cout << "Start again\n";
			exit(1);
		}
	}
	catch (const fs::filesystem_error& error) {
		std::cout << "Exception!\n";
		std::cout << error.what() << "\n"; // Looks bad sometimes
		exit(1);
	}

	bool confirmation;

	std::cout << "Run small tests and quit? (1 for yes)\n";
	std::cin >> confirmation;
	if (std::cin.fail()) {
		std::cout << "Wrong input, quitting.\n";
		exit(1);
	}
	if (confirmation) {
		test_small_bnd((tests_dir).string());
	}

	std::cout << "Should big tests be run? (1 for yes)\n";
	std::cin >> confirmation;
	if (std::cin.fail()) {
		std::cout << "Wrong input, quitting.\n";
		exit(1);
	}
	if (!confirmation) {
		std::cout << "Then quitting!\n";
		exit(0);
	}

	std::ofstream output;
	std::cout << "Print logs to screen?\n";
	std::cin >> confirmation;
	if (std::cin.fail()) {
		std::cout << "Wrong input, quitting.\n";
		exit(1);
	}
	bool use_cout = confirmation;
	if (!use_cout) {
		fs::path source;
		std::cout << "Enter the path to log big tests\n";
		std::cin >> source;
		output.open(source);
		if (!output) {
			std::cout << "Can't use" << source << "\n";
			std::cout << "Standard output will be used\n";
			use_cout = true;
		}
	}
	
	BND Data_CMN;
	std::cout << "Loading read-only vanilla_Data_cmn.bnd... This may take a while...\n";
	try {
		if (!Data_CMN.load((tests_dir / "vanilla_Data_cmn.bnd").string())) {
			"Data_CMN.load(...) claims the file was not loaded due to some error\n";
			std::cout << "Tests aborted!\n";
			exit(1);
		}
	}
	catch (...) {
		std::cout << "Error occured, please check library's report!";
		exit(1);
	}

	std::cout << "Read-only vanilla_Data_cmn.bnd successfully loaded, no exceptions!\n";

	log("Data_CMN.count_files() returns", use_cout ? std::cout : output, " ");
	log(std::to_string(Data_CMN.count_files()), use_cout ? std::cout : output, "\n\n");

	log("Data_CMN.count_entries() returns", use_cout ? std::cout : output, " ");
	log(std::to_string(Data_CMN.count_entries()), use_cout ? std::cout : output, "\n\n");
	log("Calling Data_CMN.list_all_files()...", use_cout ? std::cout : output, "\n\n");

	Data_CMN.list_all_files();
	std::cout << "\n";

	log("Trying to iterate over files...", use_cout ? std::cout : output, "\n");

	int k = 0;
	for (const auto& file : Data_CMN.files) {
		++k;
		log("Filesystem object number", use_cout ? std::cout : output, " ");
		log(std::to_string(k), use_cout ? std::cout : output, "\n");
		int level = static_cast<int> (file.level);
		if (level < 0) {
			log("Object is a file entry", use_cout ? std::cout : output, "\n");
			log("File's level:", use_cout ? std::cout : output, " ");
			log(std::to_string(level), use_cout ? std::cout : output, "\n");
			log("File's name:", use_cout ? std::cout : output, " ");
			log(file.name, use_cout ? std::cout : output, "\n");
		}
		else {
			log("Object is a directory entry", use_cout ? std::cout : output, "\n");
			log("Directory's level:", use_cout ? std::cout : output, " ");
			log(std::to_string(level), use_cout ? std::cout : output, "\n");
			log("Directory's name:", use_cout ? std::cout : output, " ");
			log(file.name, use_cout ? std::cout : output, "\n");
		}

	}
	log("\n", use_cout ? std::cout : output, "");
	log("Calling Data_CMN.list_sorted_via_offset()", use_cout ? std::cout : output, "\n");
	Data_CMN.list_sorted_via_offset();

	log("Extract tests:", use_cout ? std::cout : output, "\n");
	

	std::cout << "Tests passed successfully!\n";
}
