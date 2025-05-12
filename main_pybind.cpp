#include <pybind11/embed.h>
#include <iostream>

namespace py = pybind11;

int main() {
    py::scoped_interpreter guard{}; // インタープリタの初期化と自動終了

    try {
        // Pythonのモジュール探索パスにカレントとmsg/を追加
        py::module_ sys = py::module_::import("sys");
        sys.attr("path").attr("insert")(0, ".");
        sys.attr("path").attr("insert")(0, "./msg");

        // script_main.greeting を取得
        py::module_ script_main = py::module_::import("script_main");
        py::object greeting = script_main.attr("greeting");

        // msg.introduction_request.IntroductionRequest を取得
        py::module_ intro_module = py::module_::import("introduction_request");
        py::object IntroClass = intro_module.attr("IntroductionRequest");

        // クラスのインスタンス生成
        py::object req = IntroClass("John", 40, std::vector<std::string>{"coding", "reading"});

        // greeting(request) を呼び出し
        py::object response_obj = greeting(req);

        // response_obj.respond() を呼び出し
        py::object response_text = response_obj.attr("respond")();
        std::cout << "Greeting response: " << std::string(py::str(response_text)) << std::endl;

        // response_obj.age を取得
        int age = response_obj.attr("age").cast<int>();
        std::cout << "Age: " << age << std::endl;

    } catch (const py::error_already_set &e) {
        std::cerr << "Pythonエラー: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}