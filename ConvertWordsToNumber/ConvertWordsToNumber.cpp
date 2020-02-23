#include <iostream>
#include <map>
#include <vector>
#include <string> 
#include <algorithm>

using namespace std;

class Converter {
private:
	//Map which translate the numbers words to digits
	std::map<std::string, unsigned long long> m_conversion;

	// Vectors which save the position of the begin and end of a number, if the input only have one number they only will have one position. 
	vector < size_t > m_numbersBeginPos;
	vector < size_t > m_numbersEndPos;

public:

	Converter() {
		m_conversion["one"] = 1;
		m_conversion["two"] = 2;
		m_conversion["three"] = 3;
		m_conversion["four"] = 4;
		m_conversion["five"] = 5;
		m_conversion["six"] = 6;
		m_conversion["seven"] = 7;
		m_conversion["eight"] = 8;
		m_conversion["nine"] = 9;
		m_conversion["ten"] = 10;
		m_conversion["eleven"] = 11;
		m_conversion["twelve"] = 12;
		m_conversion["thirteen"] = 13;
		m_conversion["fourteen"] = 14;
		m_conversion["fifteen"] = 15;
		m_conversion["sixteen"] = 16;
		m_conversion["seventeen"] = 17;
		m_conversion["eighteen"] = 18;
		m_conversion["nineteen"] = 19;
		m_conversion["twenty"] = 20;
		m_conversion["thirty"] = 30;
		m_conversion["forty"] = 40;
		m_conversion["fifty"] = 50;
		m_conversion["sixty"] = 60;
		m_conversion["seventy"] = 70;
		m_conversion["eighty"] = 80;
		m_conversion["ninety"] = 90;
		m_conversion["hundred"] = 100;
		m_conversion["thousand"] = 1000;
		m_conversion["million"] = 1000000;
		m_conversion["billion"] = 1000000000;
		m_conversion["trillion"] = 1000000000000;
		m_conversion["quadrillion"] = 1000000000000000;
		m_conversion["quintillion"] = 1000000000000000000;
	}

	//Change the character ch1 to a character ch2 for replacing symbols.
	string replaceChar(string str, char ch1, char ch2) {
		for (int i = 0; i < str.length(); ++i) {
			if (str[i] == ch1)
				str[i] = ch2;
		}

		return str;
	}

	// Function which transform the original text into a vector of strings
	vector< string > Splitter(const string& a_originalSentence, char a_characterSplitter) {
		m_numbersBeginPos.clear();
		m_numbersEndPos.clear();

		//Add an character blank if didn't end the sentence with one for future purpose
		vector< string > vSplit;
		string sentence = a_originalSentence;
		//Add an character blank if didn't end the sentence with one for future purpose
		sentence = sentence + " ";

		//Replace - for numbers like twenty-seven
		sentence = replaceChar(sentence, '-', ' ');

		std::size_t current, previous = 0;
		current = sentence.find(a_characterSplitter);
		int consecutiveNumber = 0;
		while (current != std::string::npos) {
			string auxSplit = sentence.substr(previous, current - previous);
			string tolowerSplit = auxSplit;

			//Tolower the string which will be find in the cinversor map
			for_each(tolowerSplit.begin(), tolowerSplit.end(), [](char& c) { c = ::tolower(c); });


			vSplit.push_back(auxSplit);

			// If the word splittered is a number it save the begin and end position of the complete number
			if ( m_conversion.find(tolowerSplit) != m_conversion.end() ) {
				if (consecutiveNumber == 0) 
					m_numbersBeginPos.push_back(vSplit.size() - 1);
				
				vSplit.pop_back();
				vSplit.push_back(tolowerSplit);
				++consecutiveNumber;
			}
			else { 
				if (consecutiveNumber > 0) { 
 					if (!auxSplit.compare("and"))
						vSplit.pop_back();
					else { 
						m_numbersEndPos.push_back(vSplit.size() - 2);
						consecutiveNumber = 0;
					}
				}
			}
			
			previous = current + 1;
			current = sentence.find(a_characterSplitter, previous);
		}
		vSplit.push_back(sentence.substr(previous, current - previous));

		// If it wasn't added a position to the end of a number when have a begin it should be added and should be at the end of the text.
		if (m_numbersEndPos.size() < m_numbersBeginPos.size())
			m_numbersEndPos.push_back(vSplit.size()-1);

		return vSplit;
	}

	//Function which transform a vector of written numbers into a number
	unsigned long long NumberBuilder(const vector< string >& a_separateInput) {
		unsigned long long iNumber = 0;

		vector <unsigned long long> transform;

		//First pass the vector of strings to raw numbers for decimals number and readeability
		for (size_t i = 0; i < a_separateInput.size(); i++) {
			if (m_conversion[a_separateInput[i]] >= 20 && m_conversion[a_separateInput[i]] <= 90) {
				transform.push_back(m_conversion[a_separateInput[i]] + m_conversion[a_separateInput[i + 1]]);
				i++;
			}
			else
				transform.push_back(m_conversion[a_separateInput[i]]);
		}

		// if the second number of a pair is greater than 100 it need to multiply in other case is needed to add. 
		for (size_t i = 0; i < transform.size() - 1; i += 2) {
			if (transform[i + 1] >= 100)
				iNumber += transform[i] * transform[i + 1];
			else
				iNumber += transform[i] + transform[i + 1];

		}

		// If is not a even vector we need to add the last position of the vector
		if (transform.size() % 2 != 0) {
			size_t last = transform.size() - 1;

			if (transform[last] > 100)
				iNumber *= transform[last];
			else
				iNumber += transform[last];
		}
	
		return iNumber;

	}

	// Main function where with a splittered text build the sentence and replace the written numbers to a digit.
	void SentenceBuilder(const vector< string >& a_separateInput, string& a_outPut) {
		a_outPut.clear();

		vector< string > auxSeparateInput;
		auxSeparateInput = a_separateInput;
		auto it = auxSeparateInput.begin();

		for (size_t i = 0; i < m_numbersBeginPos.size(); ++i) {
			vector< string > onlyNumber(it + m_numbersBeginPos[i], it + m_numbersEndPos[i] + 1);
			size_t sizeBeforeErase = auxSeparateInput.size();

			auxSeparateInput.erase(it + m_numbersBeginPos[i], it + m_numbersEndPos[i] + 1);
			
			// If the sentece are only a number it have problem with the iterator access.
			if (auxSeparateInput.size())
				auxSeparateInput.insert(it + m_numbersBeginPos[i], to_string(NumberBuilder(onlyNumber)));
			else
				auxSeparateInput.push_back(to_string(NumberBuilder(onlyNumber)));

			//If there are two different number in the sentence we need to recalculate the actual position of the next number after erase all the words of the previous number
			if (i < m_numbersBeginPos.size() - 1) {
				m_numbersBeginPos[i + 1] -= (sizeBeforeErase - auxSeparateInput.size());
				m_numbersEndPos[i + 1] -= (sizeBeforeErase - auxSeparateInput.size());
			}
		}

		for (auto it = auxSeparateInput.begin(); it != auxSeparateInput.end(); ++it)
			a_outPut += *it + ' ';
	}
};

int main()
{
	string sInput;
	Converter converter;
	string outPut;

	cout << "Introduce the text you want to transform the numbers written with words to a number in digit (If you don't introduce anything it will show a test battery): " << endl;
	getline(cin, sInput);
	
	if (!sInput.size()) {
		converter.SentenceBuilder(converter.Splitter(sInput, ' '), outPut);
		cout << endl << outPut << endl << endl;
	}
	else {
		string sInput = "I have ten million one thousand forty two apples and twenty-seven pears ";
		string sInput1 = "I have one hundred million apples";
		string sInput2 = "I have one hundred forty two apples";
		string sInput3 = "I have two hundred apples and twenty";
		string sInput4 = "I have three hundred million apples";
		string sInput5 = "I have forty three thousand and one hundred apples ";
		string sInput6 = "I have FORTY three ThouSand and one HUNDRED seventy eight apples and two pears.";

		converter.SentenceBuilder(converter.Splitter(sInput, ' '), outPut);
		cout << sInput << endl << outPut << endl << endl;

		converter.SentenceBuilder(converter.Splitter(sInput1, ' '), outPut);
		cout << sInput1 << endl << outPut << endl << endl;

		converter.SentenceBuilder(converter.Splitter(sInput2, ' '), outPut);
		cout << sInput2 << endl << outPut << endl << endl;

		converter.SentenceBuilder(converter.Splitter(sInput3, ' '), outPut);
		cout << sInput3 << endl << outPut << endl << endl;

		converter.SentenceBuilder(converter.Splitter(sInput4, ' '), outPut);
		cout << sInput4 << endl << outPut << endl << endl;

		converter.SentenceBuilder(converter.Splitter(sInput5, ' '), outPut);
		cout << sInput5 << endl << outPut << endl << endl;

		converter.SentenceBuilder(converter.Splitter(sInput6, ' '), outPut);
		cout << sInput6 << endl << outPut << endl << endl;
	}
}