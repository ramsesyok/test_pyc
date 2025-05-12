#include <Python.h>
#include <iostream>
#include <string>

int main()
{
    // Pythonインタープリタを初期化
    Py_Initialize();

    // Pythonパスに現在のディレクトリを追加
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");

    // script_mainモジュールをインポート
    PyObject *pModule = PyImport_ImportModule("script_main");
    if (!pModule)
    {
        PyErr_Print();
        std::cerr << "script_mainモジュールの読み込みに失敗しました" << std::endl;
        Py_Finalize();
        return 1;
    }

    // script_mainモジュールからgreeting関数を取得
    PyObject *pFunc = PyObject_GetAttrString(pModule, "greeting");
    if (!pFunc || !PyCallable_Check(pFunc))
    {
        PyErr_Print();
        std::cerr << "greeting関数の取得に失敗しました" << std::endl;
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    // IntroductionRequestクラスをインポート
    PyObject *pClassModule = PyImport_ImportModule("introduction_request");
    if (!pClassModule)
    {
        PyErr_Print();
        std::cerr << "introduction_requestモジュールの読み込みに失敗しました" << std::endl;
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    PyObject *pClass = PyObject_GetAttrString(pClassModule, "IntroductionRequest");
    if (!pClass || !PyCallable_Check(pClass))
    {
        PyErr_Print();
        std::cerr << "IntroductionRequestクラスの取得に失敗しました" << std::endl;
        Py_DECREF(pClassModule);
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    // IntroductionRequestのインスタンスを作成
    PyObject *pReqArgs = Py_BuildValue("(s,i,[s,s])", "John", 30, "coding", "reading");
    PyObject *pReq = PyObject_CallObject(pClass, pReqArgs);
    Py_DECREF(pReqArgs);
    Py_DECREF(pClassModule);

    if (!pReq)
    {
        PyErr_Print();
        std::cerr << "IntroductionRequestインスタンスの作成に失敗しました" << std::endl;
        Py_DECREF(pClass);
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    // greeting関数の引数を作成
    PyObject *pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, pReq); // pReqの参照はここで移譲される

    // greeting関数を呼び出し
    PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
    if (pValue != nullptr)
    {
        // GreetingResponseオブジェクトのrespondメソッドを呼び出し
        PyObject *pRespond = PyObject_GetAttrString(pValue, "respond");
        if (pRespond && PyCallable_Check(pRespond))
        {
            PyObject *pResponseValue = PyObject_CallObject(pRespond, nullptr);
            if (pResponseValue)
            {
                std::cout << "Greeting response: " << PyUnicode_AsUTF8(pResponseValue) << std::endl;
                Py_DECREF(pResponseValue);
            }
            else
            {
                PyErr_Print();
            }
            Py_DECREF(pRespond);
        }
        else
        {
            PyErr_Print();
        }

        // GreetingResponseオブジェクトからage属性を取得
        PyObject *pAge = PyObject_GetAttrString(pValue, "age");
        if (pAge && PyLong_Check(pAge))
        {
            std::cout << "Age: " << PyLong_AsLong(pAge) << std::endl;
            Py_DECREF(pAge);
        }
        else
        {
            PyErr_Print();
        }

        Py_DECREF(pValue);
    }
    else
    {
        PyErr_Print();
    }

    // 後処理
    Py_DECREF(pArgs);
    Py_DECREF(pClass);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    // Pythonインタープリタを終了
    Py_Finalize();

    return 0;
}