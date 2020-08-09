#include <bits/stdc++.h>
#include <dirent.h>

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std;

typedef unsigned long long ull;

class Detail{
public:
    string fileName, type;
    unsigned long long size, hash;
    bool isVideoFile;
    Detail(string name, string t, unsigned long long s, unsigned long long h, bool b){
        fileName = name;
        type = t;
        size = s;
        hash = h;
        isVideoFile = b;
    }
    void print(){
        printf("File Name: %s\n", fileName.c_str());
        printf("Type: %s\n", type.c_str());
        printf("Size: %llu\n", size);
        printf("Hash: %16I64x\n\n", hash);
    }
};

addToHtml(int pos, Detail d){
    printf("<p>\n");
    printf("%d. ", pos);
    printf("%s <br>\n", d.fileName.c_str());
//    printf("File Size: %llu byte<br>\n", ceil(d.size/8.0));
//    printf("File Hash: %16I64x<br>\n", d.hash);
    printf("Download Link 1: <a href = \"https://www.opensubtitles.org/en/search/sublanguageid-all/moviehash-%16I64x\">Subtitle</a> (With hash only)<br>\n", d.hash);
    printf("Download Link 2: <a href = \"https://www.opensubtitles.org/en/search/sublanguageid-all/moviebytesize-%I64d/moviehash-%16I64x\">Subtitle</a> (With hash and Size)<br>\n", d.size, d.hash);
    printf("</p>");
}

string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}

unsigned long long fileSize(char fileName[]){
    FILE * handle = fopen(fileName, "rb");
    if(handle == NULL){
        cout<<"Failed to open File!"<<endl;
        return 0;
    }
    fseek(handle, 0, SEEK_END);
    unsigned long long size = ftell(handle);
    fclose(handle);
    return size;
}

vector<string> split (const string s, char regex){
    vector<string> ans;
    stringstream ss (s);
    string item;
    while(getline(ss, item, regex)){
        ans.push_back(item);
    }
    return ans;
}

string fileType(const string file_name){
    vector<string> v = split(file_name, '.');
    int ll = v.size();
    if(ll < 2) return "ERROR";
    else return v[ll-1];
}

bool isVideoFile(const string file_name){
    vector<string> formats {"webm",
                            "mkv",
                            "flv",
    "vob",
    "ogv",
    "ogg",
    "drc",
    "gif",
    "gifv",
    "mng",
    "avi",
    "mts",
    "m2ts",
    "ts",
    "mov",
    "qt",
    "wmv",
    "yuv",
    "rm",
    "rmvb",
    "viv",
    "asf",
    "amv",
    "mp4",
    "m4p",
    "m4v",
    "mpg",
    "mp2",
    "mpeg",
    "mpe",
    "mpv",
    "mpg",
    "mpeg",
    "m2v",
    "m4v",
    "svi",
    "3gp",
    "3g2",
    "mxf",
    "roq",
    "nsv",
    "flv",
    "f4v",
    "f4p",
    "f4a",
    "f4b",
    "zzz"
    };
    string format = fileType(file_name);
    vector<string> :: iterator it;
    it = find(formats.begin(), formats.end(), format);
    if(it!=formats.end()){
        return true;
    }else return false;
}

createHTML(string path){
    freopen("SubtitleLink.html", "w", stdout);
    printf("<html>\n");
    printf("<head>\n");
    printf("<title>SUBTITLE LINK</title>\n");
    printf("</head>\n");
    printf("<body>\n");
    printf("<h1>Subtitles</h1>\n");
    printf("<p>Directory: %s</p>\n", path.c_str());
}
endHTML(){
    printf("<br><br><br><br>");
    printf("<p>");
    printf("Powerd By: ");
        printf("<a href=\"");
        printf("https://www.opensubtitles.org");
        printf("\">OpenSubtitle.org</a>\n");
        printf("</p>");

    printf("</body></html>\n");
}

ull calculateHash(char file_name[]){
    ull CHUNK_SIZE = 1024*64;
    FILE * handle = NULL;
    handle = fopen(file_name, "rb");
    if(handle == NULL){
        printf("Failed to open the file! %s\n", file_name);
        return 0;
    }
    fseek(handle, 0, SEEK_END);
    ull size = ftell(handle);
    ull hash = size;

    fseek(handle, 0, SEEK_SET);
    for(int i=0; i<CHUNK_SIZE/(sizeof(uint64_t)); i++){
        ull a = 0;
        if(!fread((char *)(&a), sizeof(a), 1, handle)) break;
        hash+=a;
    }

    fseek(handle, max((ull) 64, size - CHUNK_SIZE), SEEK_SET);
    for(int i=0; i<(CHUNK_SIZE/sizeof(ull)); i++){
        ull a = 0;
        if(!fread((char *)(&a), sizeof(a), 1, handle)) break;
        hash+=a;
    }

    fclose(handle);

    return hash;
}



main() {
//   cout << get_current_dir() << endl;
   struct dirent *de;
   DIR *dr = opendir(".");
   vector<Detail> detailes;

    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return 0;
    }
    while ((de = readdir(dr)) != NULL){
//        printf("File Name: %s\n", de->d_name);
//        long type = de->d_type;
//        printf("File Type: %s (%ld)\n\n\n", type == 0 ? "File" : "Directory", type);
        ull size = fileSize(de->d_name);
//            printf("File Size: %llu\n", size);
            string type = fileType(de -> d_name);
//            cout<<"File Type: "<<type<<endl;
            bool isVideo = isVideoFile(de -> d_name);
//            printf("Is Video: %s\n", isVideo?"true":"false");
            ull hash = 0;
            if(isVideo){
                hash = calculateHash(de -> d_name);
            }
//            cout<<"\n\n\n";
            detailes.push_back(Detail(de -> d_name, type, size, hash, isVideo));
    }
//
//    cout<<"\n\n\n\n\n\n\n\n\n";
    createHTML(get_current_dir());
    int pos = 1;
    for(int i=0; i<detailes.size(); i++){
        if(detailes[i].isVideoFile){
            addToHtml(pos++, detailes[i]);
        }
    }
    endHTML();


    closedir(dr);
    return 0;
}

