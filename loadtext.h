 ifstream in("readme.txt", ios::in);
 istreambuf_iterator<char> beg(in), end;
 string strdata(beg, end);
 in.close();
