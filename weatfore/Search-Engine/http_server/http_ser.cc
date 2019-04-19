#include <iostream>
#include "httplib.h"
#include "../searcher/searCher.h"

int main()
{
	searCher::Searcher s;
	bool ret = s.Init("../data/tmp/raw_input");
	if(!ret) {
		std::cout << "search init fail" << std::endl;
		return 1;
	}
	httplib::Server svr;
	svr.Get("/search", [&s](const httplib::Request& req, httplib::Response& rsp) {
				std::string query = req.get_param_value("query");
				std::string result;
				s.Search(query, result);
				rsp.set_content(result, "text/plain");
			});
	svr.listen("0.0.0.0", 20000);
	return 0;
}
