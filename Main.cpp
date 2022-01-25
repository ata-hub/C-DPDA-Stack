#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <stack>
using namespace std;

void printStack(stack<string> dpdastack, fstream &f,string filename){
    //f.open(filename,ios::app);
    stack<string> tempstack;
    f<<"[STACK]:";
    if(dpdastack.empty()){
        f<<"\n";
    }
    while(!dpdastack.empty()){
        tempstack.push(dpdastack.top());
        dpdastack.pop();
    }
    while(!tempstack.empty()){
        if(tempstack.size()==1){
            f<<tempstack.top()<<"\n";
            tempstack.pop();
        }
        else{
        f<<tempstack.top()<<","; //son satırda ekstra bir boşluk bırakacak ona dikkat et, string oluşturarak onu editleyebilirsin
        tempstack.pop();
        }
    }

    //f.close();
}
bool inList(list<string> &myList,string val){
    if(val=="e"){
        return true;
    }
    for(auto const &a:myList){
        if(a==val){
            return true;
        }
    }

    return false;
}


int main(int argc, char **argv)
{
    bool noErrorDpda=true;

    string dpdafile=argv[1];
    string inputfile=argv[2];
    string outputfile=argv[3];
    string line;
    string startstate;
    list <string> dpdaInfo;
    list <string> inputInfo;
    stack<string> dpdastack;
    list<string> qstates;
    list<string> finalstates;
    list<string> alphabet;
    list<string> stackletters;
    list<string> transitions;
    stack<string> mainStack;
    int qstatesSize=0;
    int alphabetSize=0;
    int stacklettersSize=0;
    int transitionsSize=0;
    fstream f;
    fstream f2;
    fstream writeFile;
    f.open(dpdafile,ios::in);

    while(getline(f,line)){
        dpdaInfo.push_back(line);
    }
    f.close();
    f2.open(inputfile,ios::in);
    while(getline(f2,line)){
        inputInfo.push_back(line);
    }
    f2.close();
    writeFile.open(outputfile,ios::out);
    for(auto const &i:dpdaInfo){
        if(i.substr(0,2)=="Q:"){
            string temp[2];
            string T;
            string delimeter="=> ";
            int counter=0;
            temp[0]=i.substr(2,i.find(delimeter)-3);
            temp[1]=i.substr(i.find(delimeter)+3,i.length());
            stringstream x(temp[0]);
            //temp[0] ı virgülle split et ve elemanları qstates listine ekle

            while (getline(x, T, ',')) {
                qstates.push_back(T);
                qstatesSize++;

            }

            //temp[1] i virgülle bölerek start state, final statesleri stokla

            //startstate=temp[1].substr(1,2);//bunu düzelt, parantezlerin arasını alarak yap, her zaman q0 cinsinde olmayacak

            stringstream y(temp[1]);
            while (getline(y, T, ',')) {
                if(counter==0){

                    startstate=T.substr(1,T.length()-2);
                    counter++;
                }
                else{

                    finalstates.push_back(T.substr(1,T.length()-2));
                    counter++;

                }
                 // print split string


            }


        }
        else if(i.substr(0,2)=="A:"){
            stringstream x(i.substr(2,i.length()));
            string T;
            while (getline(x, T, ',')) {
            /* X represents to read the string from stringstream, T use for store the token string and,
            ' ' whitespace represents to split the string where whitespace is found. */
                alphabet.push_back(T);
                alphabetSize++;

            }
        }
        else if(i.substr(0,2)=="Z:"){
            stringstream x(i.substr(2,i.length()));
            string T;
            while (getline(x, T, ',')) {
            /* X represents to read the string from stringstream, T use for store the token string and,
            ' ' whitespace represents to split the string where whitespace is found. */
                stackletters.push_back(T);
                stacklettersSize++;

            }

        }
        else if(i.substr(0,2)=="T:"){
                transitions.push_back(i.substr(2,i.length()));
                //cout<<i.substr(2,i.length())<<endl;
                transitionsSize++;
        }


    }
    //check if transition lines are fitting the alphabet and states are fitting the state lists
    for(auto const &a:transitions){
        string  transinfo[5];
        stringstream x(a);
        string T;
        int counter=0;
        while (getline(x, T, ',')) {

            transinfo[counter]=T;

            counter++;
            }

        if(inList(qstates,transinfo[0])==true && inList(qstates,transinfo[3])==true){

        }
        else{
            //writeFile<<"File is incorrect, state not found"<<endl;
            noErrorDpda=false;
        }
        if(inList(alphabet,transinfo[1])==true){

        }
        else{
            //writeFile<<"File is incorrect, input not found"<<endl;
            noErrorDpda=false;
        }
        if(inList(stackletters,transinfo[2])==true && inList(stackletters,transinfo[4])==true){

        }
        else{
            //writeFile<<"File is incorrect, stackletter not found"<<endl;
            noErrorDpda=false;
        }



    }



    for(auto const &a:transitions){

        string  transinfo[5];
        stringstream x(a);
        string T;
        int counter=0;
        while (getline(x, T, ',')) {

            transinfo[counter]=T;

            counter++;
            }

        //qstates listinin içinde transinfo[0] ve tansinfo[3] yoksa noErrorDpda false olacak





    //now process the input lines
    if(noErrorDpda==false){
        writeFile<<"Error [1]:DPDA description is invalid!"<<endl;

    }

    if(noErrorDpda==true){
    for(auto const &inputline:inputInfo){
        list <string> inputs;
        string T;
        stringstream x(inputline);
        string nextState;
        //input letters in the line splitted with a comma is added to the list
        while (getline(x, T, ',')) {

                inputs.push_back(T);

            }


        //if the line is empty, print accept
        if(inputs.empty()==true){

           for(auto const &trans:transitions){
            //store the 5 commands of the transition in another list
            string  transinfos[5];
            stringstream x(trans);
            string T;
            int counter=0;
            while (getline(x, T, ',')) {

                transinfos[counter]=T;

                counter++;
            }
            if(startstate==transinfos[0]){
                //transinfos[1] will consume the input
                if(transinfos[1]=="e"){
                    //transinfos[2] will always be e, since it can't pop anything from empty stack

                    //nextState will be transinfos[3]
                    nextState=transinfos[3];
                    //push transinfos[4] to the stack
                    mainStack.push(transinfos[4]);
                    //cout<<"Printing first transition"<<endl;
                    writeFile<<transinfos[0]<<","<<transinfos[1]<<","<<transinfos[2]<<" => "<<transinfos[3]<<","<<transinfos[4]<<" ";
                    printStack(mainStack,writeFile,outputfile);
                    writeFile<<"ACCEPT\n\n";
                }
                else{
                    writeFile<<"ACCEPT\n\n";
                }


            }
        }





        }
        else{
        //first execute the start state
        for(auto const &trans:transitions){
            //store the 5 commands of the transition in another list
            string  transinfos[5];
            stringstream x(trans);
            string T;
            int counter=0;
            while (getline(x, T, ',')) {

                transinfos[counter]=T;

                counter++;
            }
            if(startstate==transinfos[0]){
                //transinfos[1] will consume the input
                if(transinfos[1]=="e"){

                }
                else{
                    inputs.pop_front();
                }

                //transinfos[2] will always be e, since it can't pop anything from empty stack

                //nextState will be transinfos[3]
                nextState=transinfos[3];
                //push transinfos[4] to the stack
                mainStack.push(transinfos[4]);
                //cout<<"Printing first transition"<<endl;
                writeFile<<transinfos[0]<<","<<transinfos[1]<<","<<transinfos[2]<<" => "<<transinfos[3]<<","<<transinfos[4]<<" ";
                printStack(mainStack,writeFile,outputfile);
            }
        }
        //continue with the rest of the input and transitions
        bool continueLoop=true;
        bool endLoopAfterOne=false;

        while(continueLoop==true){

        bool perfectTransition=false;//all three tansinfos match
        bool secondBestTransition=false;//next state and input matches, pop is e;
        bool lastFittingTransition=false;//next state matches, input and pop is e;
        string bestTrans[5];
        string secondBestTrans[5];
        string lastFittingTrans[5];
        for(auto const &trans:transitions){
            //store the 5 commands of the transition in another list
            string  transinfos[5];
            stringstream x(trans);
            string T;
            int counter=0;
            while (getline(x, T, ',')) {

                transinfos[counter]=T;

                counter++;
            }
            //check if transition[0] is the nextState, transition[1] matching with the first element of the input or e,

            if(transinfos[0]==nextState){

                if(transinfos[1]==inputs.front()){

                    if(transinfos[2]==mainStack.top()){
                        //This state is the most priority
                        perfectTransition=true;
                        for(int i=0;i<5;i++){
                        bestTrans[i]=transinfos[i];
                        }


                    }
                    else if(transinfos[2]=="e"){
                        secondBestTransition=true;
                        for(int i=0;i<5;i++){
                        secondBestTrans[i]=transinfos[i];
                        }

                    }
                }
                else if(transinfos[1]=="e"){
                    lastFittingTransition=true;

                    for(int i=0;i<5;i++){
                       lastFittingTrans[i]=transinfos[i];
                    }

                }

            }

        }
        //buraya if koşulları
        if(perfectTransition==true){
            //consume the first element of input list
            inputs.pop_front();
            mainStack.pop();
            nextState=bestTrans[3];
            if(bestTrans[4]!="e"){
            mainStack.push(bestTrans[4]);
            }
            //best trans elemanlarını yazdır
            for(int k=0;k<5;k++){
               if(k==2){
                    writeFile<<bestTrans[k]<<" => ";

               }
               else if(k==4){
                    writeFile<<bestTrans[k]<<" ";

               }
               else{
               writeFile<<bestTrans[k]<<",";
               }
            }
            printStack(mainStack,writeFile,outputfile);
            //if ile final state mi diye çek et, öyleyse continueLoop değerini false yap
            for(auto const &i:finalstates){
                if(i==bestTrans[3]){
                   continueLoop=false;
                }
            }
        }
        else if(secondBestTransition==true){
            //consume the input
            inputs.pop_front();

            nextState=secondBestTrans[3];
            if(secondBestTrans[4]!="e"){
            mainStack.push(secondBestTrans[4]);
            }
            //second best elemanlarını yazdır
            for(int k=0;k<5;k++){
               if(k==2){
                    writeFile<<secondBestTrans[k]<<" => ";

               }
               else if(k==4){
                    writeFile<<secondBestTrans[k]<<" ";

               }
               else{
               writeFile<<secondBestTrans[k]<<",";
               }
            }
            printStack(mainStack,writeFile,outputfile);
            //if ile final state mi diye çek et öyleyse continueLoop değerini false yap
            for(auto const &i:finalstates){
                if(i==secondBestTrans[3]){
                   continueLoop=false;
                }
            }
        }
        else if(lastFittingTransition==true){
            //bu input consume etmiyor çünkü input line e

            if(lastFittingTrans[2]!="e"){
                mainStack.pop();
            }
            nextState=lastFittingTrans[3];
            if(lastFittingTrans[4]!="e"){
            mainStack.push(lastFittingTrans[4]);
            }
            //lastfitting elemanlarını yazdır
            for(int k=0;k<5;k++){
               if(k==2){
                    writeFile<<lastFittingTrans[k]<<" => ";

               }
               else if(k==4){
                    writeFile<<lastFittingTrans[k]<<" ";

               }
               else{
               writeFile<<lastFittingTrans[k]<<",";
               }
            }
            printStack(mainStack,writeFile,outputfile);
            //if ile final state mi diye çek et öyleyse continueLoop değerini false yap
            for(auto const &i:finalstates){
                if(i==lastFittingTrans[3]){
                   continueLoop=false;
                }
            }

        }
        else{
            //writeFile<<"REJECT"<<endl;
        }
        if(inputs.empty()==true){
            if(endLoopAfterOne==true){
                continueLoop=false;
            }
            endLoopAfterOne=true;
        }

        }
        //Stack boş ise veya son state in pushladığı değer ise ACCEPT
        if(mainStack.empty()==true){
            writeFile<<"ACCEPT\n\n";
        }
        else if(mainStack.size()==1 && mainStack.top()=="$"){
            writeFile<<"ACCEPT\n\n";

        }
        else{
            writeFile<<"REJECT\n\n";

        }


    }
        //empty the stack
        while(!mainStack.empty()){
            mainStack.pop();
        }




    }
    }
    return 0;
}
}
