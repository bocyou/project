#include "searCher.h"

#if 0
int main()
{
	searCher::Index index;
	bool ret = index.bulidIndex("../data/tmp/raw_input");
	if(!ret) {
		std::cout << "build err" << std::endl;
		return 1;
	}

	auto* inverted_list = index.getInvertedList("filesystem");
	if(inverted_list == NULL) {
		std::cout << "search err" << std::endl;
		return 1;
	}

	for(auto weight : *inverted_list) {
		std::cout << "id : " << weight.doc_id << "; weight : " << weight.weight << std::endl;
		const auto* doc_info = index.getDocInfo(weight.doc_id);
		std::cout << "title: " << doc_info->title << std::endl;
		std::cout << "url: " << doc_info->url << std::endl;
		// std::cout << "content: " << doc_info->content << std::endl;
	}
	return 0;
}
#endif

int main()
{
	searCher::Searcher searcher;
	bool ret = searcher.Init("../data/tmp/raw_input");
	if(!ret) {
		return -1;
	}

	std::string query = "filesystem";
	std::string result;
	searcher.Search(query, result);
	std::cout << result << std::endl;
	return 0;
}
