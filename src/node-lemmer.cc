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
        Lemmer* lemmer = new Lemmer();
        lemmer->Wrap(args.This());
        return args.This();
    }

    Lemmer()
        : ObjectWrap()
    {
        string dict = "/Users/grachev/src/turglem-russian-0.2/dict_russian.auto";
        string predict = "/Users/grachev/src/turglem-russian-0.2/prediction_russian.auto";
        string paradigms = "/Users/grachev/src/turglem-russian-0.2/paradigms_russian.bin";

        load_lemmatizer(dict.c_str(), paradigms.c_str(), predict.c_str());
    }

    virtual ~Lemmer() throw() {}

    static Handle<Value> Lemmatize(const Arguments& args) {
        Lemmer* lemmer = ObjectWrap::Unwrap<Lemmer>(args.This());
        HandleScope scope;
        cout << "Lemmatize" << endl;

        if (args.Length() != 1 || !args[0]->IsString())
            return ThrowException(Exception::TypeError(String::New("Wrong args")));

        Local<String> str = args[0]->ToString();
        lemmer->Lemmatize(**str);
    }

    void Lemmatize(String& str) {

        vector<unsigned char> chars(str.Utf8Length());
        str.WriteUtf8((char*)&chars[0]);

        string s(chars.begin(), chars.end());
        cout << "PARAM: " << s << endl;

        tl::lem_result lr;
        size_t count = lemmatize<russian_utf8_adapter>(s.c_str(), lr);
        cout << "RES: " << count << endl;
        for (size_t i = 0; i < count; i++) {
            uint32_t srcForm = get_src_form(lr, i);
            size_t fc = get_form_count(lr, i);
            cout << "FC: " << fc << " " << (int)get_part_of_speech(lr, i, srcForm) << endl;
            for (size_t j = 0; j < fc; j++) {
                cout << "FORM: " << get_text<russian_utf8_adapter>(lr, i, j) << endl;
            }
        }
    }
};

extern "C" {
	static void init(Handle<Object> target) {
		Lemmer::Initialize(target);
	}

	NODE_MODULE(node_lemmer, init)
}
