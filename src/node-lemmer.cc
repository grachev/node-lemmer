#include <v8.h>
#include <node.h>
#include <iostream>
#include <vector>

#include <unistd.h>

#include <turglem/lemmatizer.hpp>
#include <turglem/russian/charset_adapters.hpp>

using namespace node;
using namespace v8;
using namespace std;

class Lemmer : public ObjectWrap, public tl::lemmatizer {
    static const size_t MAX_WORD_LEN = 1000;

public:
	static void Initialize(Handle<Object> target) {
        HandleScope scope;
        Local<FunctionTemplate> t = FunctionTemplate::New(New);
        t->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(t, "lemmatize", Lemmatize);

        target->Set(String::NewSymbol("Lemmer"), t->GetFunction());
    }

protected:
    static Handle<Value> New(const Arguments& args) {
        HandleScope scope;

         if (args.Length() < 2 || !args[0]->IsString() || !args[1]->IsString())
            return ThrowException(Exception::TypeError(String::New("Wrong args")));

        string dict = ToStdString(args[0]);
        string paradigms = ToStdString(args[1]);
        string predict;
        if (args.Length() == 3 && args[2]->IsString())
            predict = ToStdString(args[2]);

        Lemmer* lemmer = new Lemmer(dict, paradigms, predict);
        lemmer->Wrap(args.This());
        return args.This();
    }

    Lemmer(const string& dict, const string& paradigms, const string& predict)
        : ObjectWrap()
    {
        load_lemmatizer(dict.c_str(), paradigms.c_str(), !predict.empty() ? predict.c_str() : NULL);
    }

    virtual ~Lemmer() throw() {}

    static Handle<Value> Lemmatize(const Arguments& args) {
        Lemmer* lemmer = ObjectWrap::Unwrap<Lemmer>(args.This());
        HandleScope scope;

        if (args.Length() != 1 || !args[0]->IsString())
            return ThrowException(Exception::TypeError(String::New("Wrong args")));

        string word = ToStdString(args[0]);
        if (word.empty())
            return ThrowException(Exception::TypeError(String::New("Passed word is empty")));
       

        vector<string> lemmas = lemmer->Lemmatize(word);

        Local<Array> res = Array::New(lemmas.size());
        for (size_t i = 0; i < lemmas.size(); ++i) {
            Local<String> lemma = String::New(lemmas[i].c_str());
            res->Set(Integer::New(i), lemma);
        }

        return scope.Close(res);
    }

    vector<string> Lemmatize(const string& word) {
        tl::lem_result lr;
        size_t count = lemmatize<russian_utf8_adapter>(word.c_str(), lr);

        vector<string> result;
        for (size_t i = 0; i < count; i++) {
            //uint32_t srcForm = get_src_form(lr, i);
            //size_t fc = get_form_count(lr, i);
            //cout << "FC: " << fc << " " << (int)get_part_of_speech(lr, i, srcForm) << endl;
            //for (size_t j = 0; j < fc; j++) {
            //    cout << "FORM: " << get_text<russian_utf8_adapter>(lr, i, j) << endl;
            //}
            result.push_back(get_text<russian_utf8_adapter>(lr, i, 0));
        }

        return result;
    }

private:
    static string ToStdString(const Local<Value>& v) {
        String::Utf8Value temp(v->ToString());
        return string(*temp);
    }

};

extern "C" {
	static void init(Handle<Object> target) {
		Lemmer::Initialize(target);
	}

	NODE_MODULE(node_lemmer, init)
}
