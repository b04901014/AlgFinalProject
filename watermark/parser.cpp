#include <iostream>
#include <cstring>
#include <iomanip>
#include <cassert>
#include <fstream>
#include <vector>

using namespace std;

size_t parser(const string& filename, size_t** &next_state, size_t** &output){
    ifstream file(filename.c_str());
    string str, r;
    size_t i=0, o=0, p=0, s=0, inputCase=1;
    while(getline(file, str)){
        size_t pos=str.find(".i");
        if(pos!=string::npos){
            str=str.substr(pos+3);
            str=str.substr(0, str.find(" "));
            for(size_t j=0; j<str.size(); ++j){
                i*=10;
                i+=str[i]-'0';
            }

            //o
            getline(file, str);
            pos=str.find(".o");
            assert(pos!=string::npos);
            str=str.substr(pos+3);
            str=str.substr(0, str.find(" "));
            for(size_t j=0; j<str.size(); ++j){
                o*=10;
                o+=str[j]-'0';
            }

            //p
            getline(file, str);
            pos=str.find(".p");
            assert(pos!=string::npos);
            str=str.substr(pos+3);
            str=str.substr(0, str.find(" "));
            for(size_t j=0; j<str.size(); ++j){
                p*=10;
                p+=str[j]-'0';
            }

            //s
            getline(file, str);
            pos=str.find(".s");
            assert(pos!=string::npos);
            str=str.substr(pos+3);
            str=str.substr(0, str.find(" "));
            for(size_t j=0; j<str.size(); ++j){
                s*=10;
                s+=str[j]-'0';
            }

            //r
            getline(file, str);
            pos=str.find(".r");
            assert(pos!=string::npos);
            str=str.substr(pos+3);
            r=str.substr(0, str.find(" "));

            break;
        }

    }
    
    for(size_t j=0; j<i; ++j) inputCase*=2;

    next_state=new size_t*[s];
    output=new size_t*[s];
    for(size_t j=0; j<s; ++j){
        next_state[j]=new size_t[inputCase];
        output[j]=new size_t[inputCase];
        for(size_t k=0; k<inputCase; ++k){
            next_state[j][k]=-1;
            output[j][k]=-1;
        }
    }

    string nextS_str, current_state, input_pre, output_str;
    size_t input, current_num, inputSize, multiplier, nextS, output_pre;
    bool dontcare[i];
    vector<size_t> inputTotal;
    for(size_t j=0; j<p; ++j){
        //input
        inputTotal.clear();
        file>>input_pre;
        input=0;
        multiplier=1;
        for(size_t k=0; k<i; ++k){
            input*=2;
            if(input_pre[k]=='-'){
                dontcare[k]=true;
            }
            else {
                dontcare[k]=false;
                input+=input_pre[k]-'0';
            }
        }
        inputTotal.push_back(input);
        for(size_t k=i-1; k<i; --k){
            inputSize=inputTotal.size();
            for(size_t l=0; l<inputSize; ++l){
                if(dontcare[k]==true) inputTotal.push_back(inputTotal[l]+multiplier);
            }
            multiplier*=2;
        }

        //current state
        file>>current_state;
        current_state=current_state.substr(1);
        current_num=0;
        for(size_t k=0; k<current_state.size(); ++k){
            current_num*=10;
            current_num+=current_state[k]-'0';
        }
        
        //next state
        file>>nextS_str;
        nextS_str=nextS_str.substr(1);
        nextS=0;
        for(size_t k=0; k<nextS_str.size(); ++k){
            nextS*=10;
            nextS+=nextS_str[k]-'0';
        }
        
        //output
        file>>output_str;
        output_pre=0;
        for(size_t k=0; k<o; ++k){
            output_pre*=2;
            output_pre+=output_str[k]-'0';
        }


        for(size_t k=0; k<inputTotal.size(); ++k){
            next_state[current_num][inputTotal[k]]=nextS;
            output[current_num][inputTotal[k]]=output_pre;
        }
    }
    return s;

} 

int main(){
    size_t **next_state=0, **output=0, s_num;
    s_num=parser("watermark/kiss/t3.kiss", next_state, output);

    for(size_t i=0; i<s_num; ++i){
        delete[] next_state[i];
        delete[] output[i];
    }
    delete[] next_state;
    delete[] output;
    return 0;
}