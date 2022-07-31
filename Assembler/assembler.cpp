#include <bits/stdc++.h>
using namespace std;

string findTwoscomplement(string str)
{
	int n = str.length();
	int i;
	for (i = n - 1; i >= 0; i--)
		if (str[i] == '1')
			break;
	if (i == -1)
		return str;
	for (int k = i - 1; k >= 0; k--)
	{
		if (str[k] == '1')
			str[k] = '0';
		else
			str[k] = '1';
	}
	return str;
	;
}


string tobinary(string s)
{
	int num = 0;
	int flag = 0;
	string str = "";
	cout << '>' << s << endl;
	for (int i = 1; i < s.length(); i++)
	{
		if (s[i] == '-')
		{
			flag = 1;
			continue;
		}
		num = num * 10 + (s[i] - '0');
	}
	if (num >= pow(2, 12))
		return "";
	for (int i = 0; i < 12; i++)
	{
		if (num % 2 == 0)
			str += "0";
		else
			str += "1";
		num /= 2;
	}
	reverse(str.begin(), str.end());
	if (flag == 1)
	{
		str = findTwoscomplement(str);
	}
	cout << '>' << str << endl;
	return str;
}

string convert_jump_label(int num)
{
	string str = "";
	if (num >= pow(2, 12))
		return "";
	for (int i = 0; i < 12; i++)
	{
		if (num % 2 == 0)
			str += "0";
		else
			str += "1";
		num /= 2;
	}
	reverse(str.begin(), str.end());
	return str;
}

string convert_branch_label(int num)
{
	int flag = 0;
	string str = "";
	if (num <= 0)
	{
		flag = 1;
		num *= -1;
	}
	if (num >= pow(2, 16))
		return "";
	for (int i = 0; i < 12; i++)
	{
		if (num % 2 == 0)
			str += "0";
		else
			str += "1";
		num /= 2;
	}
	reverse(str.begin(), str.end());
	if (flag == 1)
	{
		str = findTwoscomplement(str);
	}
	return str;
}

string convert_shift_label(string s)
{
	int num = 0;
	string str = "";
	for (int i = 0; i < s.length(); i++)
	{
		num = num * 10 + (s[i] - '0');
	}
	if (num >= pow(2, 5))
		return "";
	for (int i = 0; i < 5; i++)
	{
		if (num % 2 == 0)
			str += "0";
		else
			str += "1";
		num /= 2;
	}
	reverse(str.begin(), str.end());
	return str;
}

int main()
{
	unordered_map<string, int> label_table;
	ifstream fin1;
	int count = 0;
	string result;
	fin1.open("arm1.s");
	ofstream fout1;
	fout1.open("machinecode1.txt");

	while (!fin1.eof()) //Label Check -----------------------
	{
		string line;
		int flag = 0;
		getline(fin1, line);
		for (int i = 0; i < line.length(); i++)
		{
			// printf("%c %d\n", line[0], i);
			if (isalnum(line[i]))
			{
				// printf("Here %d", isalnum(line[i]));
				flag = 1;
				break;
			}
		}
		if (flag == 0)
			continue;
		if (line[line.length() - 1] == ':')
		{
			line.erase(line.length() - 1, line.length());
			label_table[line] = count;
			continue;
		}
		count++;
	}
	fin1.close();
	fin1.open("arm1.s");
	count = 0;
	while (!fin1.eof())
	{
		string info[3] = {"", "", "0"};  // 0- cond, 1- opcode + s, 2- I
		string r[3] = {"0000", "0000", ""};
		string command = "";
		int pos, flag = 0;
		string line;
		getline(fin1, line);

		for (int i = 0; i < line.length(); i++)
		{
			if (isalnum(line[i]))
			{
				flag = 1;
				break;
			}
		}
		if (flag == 0)
			continue;
		flag = 0;
		if (line[0] == '\t' || line[0] == ' ')
		{
			line.erase(0, 1);
		}
		// cout  << line.length() << endl;
		if (line[line.length() - 2] == ':')
		{
			continue;
		}
		count++;
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] == '\t' || line[i] == ' ')
			{
				// cout << line << endl;
				pos = i + 1;
				break;
			}
			else
				command += line[i];
		}
		ifstream fin2;
		fin2.open("symbol table for assembly.txt");
		while (fin2)
		{
			string match;
			getline(fin2, match);
			cout << match << " " << command << endl;
			if (match == command)
			{
				flag = 1;
				getline(fin2, match); // 000000	100000	00000
				string temp1 = "";
				int k = 0;
				for (int i = 0; i < match.length(); i++)
				{
					if (match[i] == ' ')
					{
						info[k++] = temp1;
						temp1 = "";
					}
					else
					{
						temp1 += match[i];
					}
				}
				info[k++] = temp1;
				//	cout<<info[0]<<info[1]<<info[2]<<endl;
				break;
			}
			getline(fin2, match);
		}
		if (flag == 0)
		{
			cout << "Syntax error in line " << count << endl;
			cout << "Specified operation is invalid\n";
			cout << line << endl;
			return 0;
		}
		flag = 0;
		fin2.close();
		int k = 0;
		string temp = "";
		for (int i = pos; i < line.length(); i++)
		{
			if (line[i] == ',')
			{
				if (temp == "" || temp == " " || temp == ",")
				{
					cout << "Syntax error in line " << count << endl;
					cout << "Additional symbols used\n";
					cout << line;
					return 0;
				}
				ifstream fin2;
				fin2.open("symbol table for assembly.txt");
				flag = 0;
				while (fin2)
				{
					string match;
					getline(fin2, match);
					// cout << match << '-' << temp << endl;
					if (match == temp)
					{
						flag = 1;
						getline(fin2, match);
						r[k++] = match;
						break;
					}
					getline(fin2, match);
				}
				if (flag == 0)
				{
					cout << "Syntax error in line " << count << endl;
					cout << "Invalid register\n";
					cout << line;
					return 0;
				}
				fin2.close();
				temp = "";
			}
			else if (line[i] != ']'){
				if(line[i] != '[')
					temp += line[i];
			}
		}

		// cout << temp << '-' << info[0] << '-' << info[1] << '-' << r[0] << '-' << r[1] << '-' << r[2] << endl;

		flag = 0;
		//ifstream fin2;
		if (isdigit(temp[1])||temp[1]=='-')
		{
			info[2] = '1';
			r[2] = tobinary(temp);
			if (r[2] == "")
			{
				cout << "Math error-beyond range in line " << count << endl;
				cout << line << endl;
				return 0;
			}
			//fout1 << info[0] << r[0] << r[1] << r[2] << info[2] << info[1] << endl;
			result = info[0] + "00" + info[2] + info[1] + r[1] + r[0] + r[2];
			fout1 << result << endl;
			continue;
		}
		if (temp == "" || temp == " " || temp == ",")
		{
			cout << "Syntax error in line " << count << endl;
			cout << "Additional symbols used\n";
			cout << line << endl;
			return 0;
		}

		if (temp[0] != 'r')
		{
			int val = label_table[temp];
			//cout << val << "," << info[0] << "," << endl;
			if (info[0] == "0001" || info[0] == "0010")
			{
				r[2] = convert_jump_label(val);
				if (r[2] == "")
				{
					cout << "Math error-beyond range in line " << count << endl;
					cout << line;
					return 0;
				}
				//fout1 << info[0] << r[0] << r[1] << r[2] << info[2] << info[1] << endl;
				result = info[0] + "00" + info[2] + info[1] + r[1] + r[0] + r[2];
				fout1 << result << endl;
				continue;
			}
			// else
			// {
			// 	int res = val - (count - 1) - 1;
			// 	//cout << count << " " << val << " " << res << endl;
			// 	r[2] = convert_branch_label(res);
			// 	if (r[2] == "")
			// 	{
			// 		cout << "Math error-beyond range in line " << count << endl;
			// 		cout << line;
			// 		return 0;
			// 	}
			// 	//fout1 << info[0] << r[0] << r[1] << info[2] << info[1] << r[2] << endl;
			// 	result = info[0] + r[0] + r[1] + info[2] + info[1] + r[2];
			// 	fout1 << result.substr(0, 8) << endl
			// 		  << result.substr(8, 8) << endl
			// 		  << result.substr(16, 8) << endl
			// 		  << result.substr(24, 8) << endl
			// 		  << result << endl;
			// 	continue;
			// }
		}
		fin2.open("symbol table for assembly.txt");
		while (fin2)
		{
			string match;
			getline(fin2, match);
			if (match == temp)
			{
				flag = 1;
				getline(fin2, match);
				r[2] = match;
				break;
			}
		}
		fin2.close();
		if (flag == 0)
		{
			cout << "Syntax error in line " << count << endl;
			cout << "Invalid Operand\n";
			cout << line << endl;
			return 0;
		}
		// printf("hi\n");
		//cout<<r[1]<<endl;
		//fout1 << info[0] << r[0] << r[1] << r[2] << info[2] << info[1] << endl;
		result = info[0] + "00" + info[2] + info[1] + r[1] + r[0] + "00000000" + r[2];
		fout1 << result << endl;
	}
	fin1.close();
	fout1.close();
}
