//#include <iostream>  
//#include <boost/asio.hpp>  
//
//int main() {
//    try {
//        boost::asio::io_context io;
//
//        // 创建一个定时器，设置超时时间为1秒  
//        boost::asio::steady_timer timer(io, std::chrono::seconds(1));
//
//        timer.async_wait([](const boost::system::error_code& /*e*/) {
//            std::cout << "Hello, Boost Asio!" << std::endl;
//            });
//
//        io.run(); // 运行I/O服务  
//    } catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//
//    return 0;
//}


//#include <iostream>
//#include <json/json.h>
//#include <json/value.h>
//#include <json/reader.h>
//
//int main() {
//	Json::Value root;
//	root["id"] = 1001;
//	root["data"] = "hello world";
//	std::string request = root.toStyledString();
//	std::cout << "request is : " << request << std::endl;
//
//	Json::Value root2;
//	Json::Reader reader;
//	reader.parse(request, root2);
//	std::cout << "msg id is: " << root2["id"] << " , msg is: " << root2["data"] << std::endl;
//	return 0;
//}


//#include <iostream>
//#include "msg.pb.h"
//int main() {
//	Book book;
//	book.set_name("CPP programing");
//	book.set_pages(100);
//	book.set_price(200);
//	std::string bookStr;
//	book.SerializeToString(&bookStr);
//	std::cout << "serialize str is: " << bookStr << std::endl;
//	Book book2;
//	book2.ParseFromString(bookStr);
//	std::cout << "book name :" << book2.name() << " , pages: " << book2.pages() << " , price: " << book2.price() << std::endl;
//	getchar();
//}


//#include <iostream>
//
//using namespace std;
//
//int main() {
//	string s;
//	int k;
//	cin >> s >> k;
//	int arr[26] = { 0 };
//	for (int i = 0; i < s.size(); ++i) {
//		++arr[(int)(s[i] - 'a')];
//	}
//	cout << endl;
//	int index = 0;
//	while (k--) {
//		while (index <= 26 && arr[index] == 0) ++index;
//		int tmp = s.size() - 1;
//		while (arr[index] > 0) {
//			while (s[tmp] != (char)(index + 'a')) --tmp;
//			s.erase(s.begin() + tmp);
//			--arr[index];
//		}
//	}
//	cout << s << endl;
//	return 0;
//}

//#include <iostream>
//#include <vector>
//#include <unordered_set>
//
//using namespace std;
//
//bool isPrime(int num) {
//	if (num < 2) return false;
//	for (int i = 2; i * i <= num; ++i) {
//		if (num % i == 0) return false;
//	}
//	return true;
//}
//
//void backtracking(vector<int>& nums, int& minRes, int currNum, unordered_set<int>& visited, unordered_set<int>& primes) {
//	if (visited.size() == nums.size() && isPrime(currNum)) {
//		primes.insert(currNum);
//		minRes = min(minRes, currNum);
//		return;
//	}
//	for (int i = 0; i < nums.size(); ++i) {
//		if (visited.find(i) != visited.end()) continue;
//		visited.insert(i);
//		backtracking(nums, minRes, (currNum * 10 + nums[i]), visited, primes);
//		visited.erase(i);
//	}
//}
//
//int main() {
//	int n;
//	cin >> n;
//	vector<int> nums;
//	while (n) {
//		int tmp = n % 10;
//		nums.push_back(tmp);
//		n /= 10;
//	}
//	int res = INT_MAX;
//	unordered_set<int> visited;
//	unordered_set<int> primes;
//	backtracking(nums, res, 0, visited, primes);
//	if (primes.empty()) cout << 0 << endl;
//	else cout << primes.size() << endl << res << endl;
//	return 0;
//}


#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int i = 0;
    while (++i > 0);
    printf("正在输出\n");
    printf("输出结束");
    return 0;
}
