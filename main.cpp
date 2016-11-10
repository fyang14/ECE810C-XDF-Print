#include"iostream"
#include "queue"
#include "map"
#include "string"
#include "iterator"
#include "vector"
#include "algorithm"
#include <fstream>
using namespace std;

#define NChar 8 //suppose use 8 bits to describe all symbols
#define Chars 1<<NChar //can describe 256 symbols totally (include a-z, A-Z,0-9)
typedef vector<bool> Huff_code;//8 bit code of one char
map<char,Huff_code> Huff_Dic; //huffman coding dictionary

class HTree
{
    public :
    HTree* left;
    HTree* right;
    char ch;
    int weight;
    
    HTree(){left = right = NULL; weight=0;ch ='\0';}
    HTree(HTree* l,HTree* r,int w,char c){left = l; right = r;  weight=w;   ch=c;}
    ~HTree(){delete left; delete right;}
    bool Isleaf(){return !left && !right; }
};

class Compare_tree
{
public:
    bool operator () (HTree* t1, HTree* t2)
    {
        return t1->weight> t2->weight;
    }
};

HTree* BuildTree(int *frequency)
{
    priority_queue<HTree*,vector<HTree*>,Compare_tree> QTree;
    
    //1st level add characters
    for (int i=0;i<Chars;i++)
    {
        if(frequency[i])
            QTree.push(new HTree(NULL,NULL,frequency[i],(char)i));
    }
    
    //build
    while (QTree.size()>1)
    {
        HTree* lc  = QTree.top();
        QTree.pop();
        HTree* rc = QTree.top();
        QTree.pop();
        
        HTree* parent = new HTree(lc,rc,lc->weight+rc->weight,(char)256);
        QTree.push(parent);
    }
    //return tree root
    return QTree.top();
}

void Huffman_Coding(HTree* root, Huff_code& curcode)
{
    if(root->Isleaf())
    {
        Huff_Dic[root->ch] = curcode;
        return;
    }
    Huff_code lcode = curcode;
    Huff_code rcode = curcode;
    lcode.push_back(false);
    rcode.push_back(true);
    
    Huffman_Coding(root->left,lcode);
    Huffman_Coding(root->right,rcode);
}
static bool cmp(pair<int,int>& p1,pair<int,int>& p2){
    
    return p1.second>p2.second;
    
}
int main()
{
    int freq[Chars] = {0};
    char ch;
    string str;
    
    
    //read the input file
    //fstream fin("/Users/weizhengkai/Desktop/huffmon code/infile.dat", fstream::in);
    fstream fin("infile.dat", fstream::in);
    ofstream myfile;
    //myfile.open ("/Users/weizhengkai/Desktop/huffmon code/outfile.dat");
    myfile.open ("outfile.dat");
    ////////////////////////////////
    if(!fin.is_open())
    {
        cout<<"can not read file"<<endl;
        return -1;
    }
    if(!myfile.is_open())
    {
        cout<<"open outfile.dat fail"<<endl;
        return -2;
    }
    //statistic character frequency
    while (fin >> noskipws >> ch) {
        if(('0'<=ch&&ch<='9')||('A'<=ch&&ch<='Z')||('a'<=ch&&ch<='z')) {
            str.push_back(ch);
            freq[ch]++;
        }
    }
    myfile<<"symbol frequency\n";
    vector<pair<int,int>> freq_pair;
    for(int i=0;i<Chars;i++)
        if(freq[i]>0)
            freq_pair.emplace_back(i,freq[i]);
    sort(freq_pair.begin(),freq_pair.end(),cmp);
    
    for(auto f:freq_pair)
        myfile<<char(f.first)<<", "<<f.second<<'\n';
    
    
    //build tree
    HTree* r = BuildTree(freq);
    Huff_code nullcode;
    nullcode.clear();
    Huffman_Coding(r,nullcode);
    myfile<<"\nHuffman code table:\n";
    int len=0;
    for(map<char,Huff_code>::iterator it = Huff_Dic.begin(); it != Huff_Dic.end(); it++)
    {
        myfile<<(*it).first<<'\t';
        len+=(freq[(*it).first]*(it->second).size());
        for(auto b:it->second)
            myfile<<int(b);
        // std::copy(it->second.begin(),it->second.end(),std::ostream_iterator<bool>(cout));
        myfile<<endl;
    }
    myfile<<"\nlength of the coded message in terms of number of bits: "<<len;
    /////////////////////////////
    fin.close();
    myfile.close();
    return 0;
}