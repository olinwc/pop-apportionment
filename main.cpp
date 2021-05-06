#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

void
hamiltonMethod(unsigned int & assigned_seats,
               unsigned int & total_seats,
               vector<unsigned int> & seat_vector,
               const vector<unsigned int> & pop_vector,
               const double & ratio)
{

  // First load all states with the number of representatives they would get under the Jefferson method
  for (unsigned int i = 0; i != pop_vector.size(); ++i)
  {
    double real_seat_value = pop_vector[i] / ratio;
    // If the real_seat_value is greater than 1, than load the calculated Jefferson value for the seat
    if (real_seat_value > 1.0)
    {
      seat_vector[i] = floor(real_seat_value);
    }
    assigned_seats += seat_vector[i];
  }
  // Maximum differences in a state
  double max_diff = 0.0;
  // Index of the state with the highest underrepresentation ratio
  unsigned int max_diff_index = 0;
  // Misapprortionment ratio for the current state
  double in_ratio = 0.0;
  // Fraction of a seat the state "deserves"
  double diff_value = 0.0;
  while (assigned_seats < total_seats)
  {
    for (unsigned int i = 0; i != pop_vector.size(); ++i)
    {
      double pop_double = static_cast<double>(pop_vector[i]);
      in_ratio = pop_double / ratio;
      double seat_double = static_cast<double>(seat_vector[i]);
      diff_value = in_ratio - seat_double;

      if (diff_value > max_diff)
      {
        max_diff = diff_value;
        max_diff_index = i;
      }
    }
    seat_vector[max_diff_index] = seat_vector[max_diff_index] + 1;
    max_diff = 0.0;
    assigned_seats++;
  }
  if (assigned_seats > total_seats)
  {
    cout << "WARNING: Allocated Hamilton method with " << assigned_seats << " rather than " << total_seats << endl;
  }
}

void
huntingtonHillMethod(unsigned int & assigned_seats,
                     unsigned int & total_seats,
                     const unsigned int & population,
                     const unsigned int & ideal_ratio,
                     vector<unsigned int> & seat_vector,
                     const vector<unsigned int> & pop_vector)
{
  // Calculated representation
  double hh_ratio = 0.0;
  // Index of the state with the highest underrepresentation ratio
  unsigned int hh_ratio_index = 0;
  double in_ratio = 0.0;
  // Number of seats already assigned is the number of states
  assigned_seats = pop_vector.size();
  while (assigned_seats < total_seats)
  {
    for (unsigned int i = 0; i != pop_vector.size(); ++i)
    {
      double seat_double = static_cast<double>(seat_vector[i]);
      double pop_double = static_cast<double>(pop_vector[i]);
      in_ratio = pop_double / pow((seat_double * (seat_double + 1.0)), 0.5);

      if (in_ratio > hh_ratio)
      {
        hh_ratio = in_ratio;
        hh_ratio_index = i;
      }
    }
    seat_vector[hh_ratio_index] = seat_vector[hh_ratio_index] + 1;
    hh_ratio = 0.0;
    assigned_seats++;
  }
  if (assigned_seats > total_seats)
    cout << "WARNING: Allocated Huntington-Hill method with " << assigned_seats << " instead of " << total_seats << "\n\n";
}

void
jeffersonMethodByRatio(unsigned int & assigned_seats,
                       vector<unsigned int> & seat_vector,
                       const vector<unsigned int> & pop_vector,
                       const unsigned int & ideal_ratio)
{
  assigned_seats = 0;
  for (unsigned int i = 0; i != pop_vector.size(); ++i)
  {
    if (pop_vector[i] > ideal_ratio)
      seat_vector[i] = pop_vector[i] / ideal_ratio;
    assigned_seats += seat_vector[i];
  }
}

int
jeffersonMethodBySeats(unsigned int & assigned_seats,
                       unsigned int & total_seats,
                       const unsigned int & population,
                       vector<unsigned int> & seat_vector,
                       const vector<unsigned int> & pop_vector,
                       const bool & debug)
{
  unsigned int j_threshold = 0;
  // Number of times the while loop is performed
  unsigned int num_jefferson_loops = 0;
  j_threshold = population / total_seats;
  while (assigned_seats < total_seats)
  {
    num_jefferson_loops++;
    for (unsigned int i = 0; i != pop_vector.size(); ++i)
    {
      if (pop_vector[i] > j_threshold)
        seat_vector[i] = pop_vector[i] / j_threshold;
    }
    assigned_seats = 0;
    for (unsigned int i = 0; i != pop_vector.size(); ++i)
    {
      assigned_seats += seat_vector[i];
    }
    if (assigned_seats > total_seats)
      cout << "WARNING: Allocated Jefferson method with " << assigned_seats << " instead of " << total_seats << "!\n\n";
    else if (assigned_seats < total_seats && j_threshold == 1)
    {
      cout << "ERROR: Cannot allocate Jefferson method with given inputs, closing program\n";
      return 1;
    }
    else
    {
      if (debug)
        cout << "Recalculating Jefferson method\n\n";
      // FIXME: Reduce the threshold by 0.25% and then recalculate
      // This may need to be adjusted in the future
      j_threshold = j_threshold * 0.9975;
    }
  }
  if (debug)
    cout << "Required " << num_jefferson_loops << " loops\n\n";
  return 0;
}

void
websterMethodByRatio(unsigned int & assigned_seats,
                     vector<unsigned int> & seat_vector,
                     const vector<unsigned int> & pop_vector,
                     const unsigned int & ratio)
{
  double webster_seats = 0.0;
  assigned_seats = 0;
  for (unsigned int i = 0; i != pop_vector.size(); ++i)
  {
    if (pop_vector[i] > ratio)
    {
      webster_seats = pop_vector[i] / ratio;
      seat_vector[i] = round(webster_seats);
    }
    assigned_seats += seat_vector[i];
  }
}

void
websterMethodBySeats(unsigned int & assigned_seats,
                     unsigned int & total_seats,
                     vector<unsigned int> & seat_vector,
                     const vector<unsigned int> & pop_vector,
                     unsigned int ratio)
{
  double webster_seats = 0.0;
  assigned_seats = 0;
  for (unsigned int i = 0; i != pop_vector.size(); ++i)
  {
    if (pop_vector[i] > ratio)
    {
      webster_seats = pop_vector[i] / ratio;
      seat_vector[i] = round(webster_seats);
    }
    assigned_seats += seat_vector[i];
  }
  // There is the possibility of not being able to match the specified number of seats with the Webster method
  if (assigned_seats > total_seats)
  {
    cout << "WARNING: Allocated Webster method with " << assigned_seats << " rather than " << total_seats << endl;
  }
  else if (assigned_seats < total_seats)
  {
    cout << "Recalculating Webster Method\n\n";
    assigned_seats = 0;
    vector<unsigned int> temp_seat_vector(pop_vector.size(), 1);
    seat_vector = temp_seat_vector;
    // FIXME: Reducing ratio by 0.25%
    // May want to change in the future
    ratio = ratio * 0.9975;
    websterMethodBySeats(assigned_seats,
                         total_seats,
                         seat_vector,
                         pop_vector,
                         ratio);
  }
}

int
main()
{
  // Declare variables
  // Whether or not to run in debug mode
  bool debug = true;
  // Numerical identifier for which apportionment method to use
  unsigned int method;
  // The .CSV file which will contain the input data
  ifstream input_file;
  // Pull from the data directory
  string data_name = "data/";
  // The name of the input file
  string input_name;
  // The .CSV file which will contain the output data
  ofstream output_file;
  // The name of the output file
  string output_name;
  // Vector of the population of each state
  vector<unsigned int> pop_vector;
  // Vector of the names of each state
  vector<string> name_vector;
  // Vector of the political bias of the state (Positive is Democrat, Negative is Republican)
  vector<int> bias_vector;
  // Vector of the lines from the input file
  vector<string> line_vector;
  // String storing a line from the input file
  string line;
  // Opening prompts for users
  cout << "This program relies on user input to specify specific parameters\n";
  cout << "If invalid values are entered, the program will terminate and need to be restarted\n";
  cout << "Hope you enjoy it :)\n\n";

  // First specify apportionment method
  cout << "Enter the number corresponding to the apportionment method you want to use\n\n";
  cout << "1: Huntington-Hill method\n";
  cout << "2: Hamilton method\n";
  cout << "3: Jefferson method\n";
  cout << "4: Webster method\n";
  cout << "9: All methods for Latex output\n";
  cout << '\n';
  cout << "Please enter a number between 1 and 4:\n\n";
  cin >> method;
  cout << '\n';
  if (method == 1)
    cout << "Huntington-Hill method selected\n\n";
  else if (method == 2)
    cout << "Hamilton method selected\n\n";
  else if (method == 3)
    cout << "Jefferson method selected\n\n";
  else if (method == 4)
    cout << "Webster method selected\n\n";
  else if (method == 9)
    cout << "Running all methods\n\n";
  else
  {
    cout << "ERROR: Invalid input of " << method << " entered, closing program\n";
    return 0;
  }
  cout << "What is the name of the data file (in .csv format)?\n\n";

  cin >> input_name;
  input_name = data_name.append(input_name);
  input_file.open(input_name);

  cout << '\n';

  if (input_file.is_open())
  {
    if (debug)
      cout << input_name << " was successfully opened\n\n";
    while (getline(input_file, line))
		{
			line_vector.push_back(line);
		}
    input_file.close();
  }
  else
  {
    cout << "ERROR: Could not open " << input_name << ". Closing the program\n";
    return 0;
  }
  // Parse the .csv input file
  for (unsigned int i = 0; i < line_vector.size(); ++i)
	{
		string to_parse = line_vector[i];
		string delimeter = ",";
		string name = to_parse.substr(0, to_parse.find(delimeter));
		to_parse.erase(0, to_parse.find(delimeter) + 1);
    string pop_string = to_parse.substr(0, to_parse.find(delimeter));
		unsigned int pop = stoi(pop_string);
    to_parse.erase(0, to_parse.find(delimeter) + 1);
    // Positive bias represents Democrat bias
    int bias = stoi(to_parse);
		name_vector.push_back(name);
		pop_vector.push_back(pop);
    bias_vector.push_back(bias);
	}

  // Total population
  unsigned int population = 0;
  for (unsigned int i = 0; i != pop_vector.size(); ++i)
		population += pop_vector[i];
  // The total number of seats to apportion
  unsigned int total_seats = 0;
  // The ideal number of people per seat
  unsigned int ideal_ratio = 0;
  cout << "Would you prefer to specify (enter the number corresponding to that option)?\n\n";

  cout << "1: The total number of seats to be apportioned (recommended for Hamilton & Huntington-Hill)\n";
  cout << "2: The ideal number of people per seat (recommended for Jefferson & Webster)\n\n";

  // Whether to use total seats or people per seat
  unsigned int option;

  cin >> option;

  cout << '\n';

  // Ideal number of people per seat
  double ratio = 0.0;
  if (option == 1)
  {
    cout << "How many total seats will there be to apportion?\n\n";
    cin >> total_seats;
    cout << '\n';
    if (total_seats == 0)
    {
      cout << "ERROR: There cannot be zero seats to apportion\n";
      return 0;
    }
    else if (total_seats < name_vector.size())
    {
      cout << "ERROR: There cannot be " << total_seats
           << " total seats since that is less than the number of states which is "
           << name_vector.size() << '\n';
      return 0;
    }
    ratio = population / total_seats;
  }
  else if (option == 2)
  {
    cout << "How many people should there be per seat?\n\n";
    cin >> ideal_ratio;
    cout << '\n';
    if (ideal_ratio >= population)
    {
      cout << "ERROR: People per seat ratio of " << ideal_ratio << " specified when total population is " << population << endl;
      return 0;
    }
    else if (ideal_ratio >= population / pop_vector.size())
    {
      cout << "WARNING: People per seat ratio of " << ideal_ratio << " is greater than the total population divided by the number of states" << "\n\n";
    }
    else if (ideal_ratio == 0)
    {
      cout << "Am I a joke to you?" << endl;
      return 0;
    }
    total_seats = population / ideal_ratio;
  }

  // Total number of seats that have been assigned
  unsigned int assigned_seats = 0;
  // Every state gets 1 seat initially
  // That's the rule in the US Constitution and is honestly a good rule in general
  // Vector of the seats belonging to each state
  vector<unsigned int> seat_vector(pop_vector.size(), 1);

  // Special declarations used by method 9
  vector<unsigned int> jefferson_seat_vector(pop_vector.size(), 1);
  vector<unsigned int> hamilton_seat_vector(pop_vector.size(), 1);
  vector<unsigned int> huntington_hill_seat_vector(pop_vector.size(), 1);
  vector<unsigned int> webster_seat_vector(pop_vector.size(), 1);

  unsigned int jefferson_assigned_seats = 0;
  unsigned int hamilton_assigned_seats = 0;
  unsigned int huntington_hill_assigned_seats = 0;
  unsigned int webster_assigned_seats = 0;

  // You could probably use a switch but if/else if statements work so...
  // Method 1: The Huntington-Hill method
  if (method == 1)
  {
    huntingtonHillMethod(assigned_seats,
                         total_seats,
                         population,
                         ideal_ratio,
                         seat_vector,
                         pop_vector);
  }
  // Method 2: The Hamilton method
  else if (method == 2)
  {
    hamiltonMethod(assigned_seats,
                   total_seats,
                   seat_vector,
                   pop_vector,
                   ratio);
  }
  // Method 3: The Jefferson method
  else if (method == 3)
  {
    unsigned int error_check = 0;
    if (option == 1)
    {
      error_check = jeffersonMethodBySeats(assigned_seats,
                                           total_seats,
                                           population,
                                           seat_vector,
                                           pop_vector,
                                           debug);
    }
    else if (option == 2)
    {
      jeffersonMethodByRatio(assigned_seats,
                             seat_vector,
                             pop_vector,
                             ideal_ratio);
    }
    // Close program if error check is greater than 0
    if (error_check > 0)
    {
      cout << "ERROR: Problem running Jefferson Method, closing program" << endl;
      return 0;
    }
  }
  // Method 4: The Webster method
  else if (method == 4)
  {
    if (option == 1)
    {
      websterMethodBySeats(assigned_seats,
                           total_seats,
                           seat_vector,
                           pop_vector,
                           ratio);
    }
    else if (option == 2)
    {
      websterMethodByRatio(assigned_seats,
                           seat_vector,
                           pop_vector,
                           ratio);
    }
  }
  else if (method == 9)
  {
    if (option == 1)
    {
      if (debug)
        cout << "Running Jefferson Method\n";
      unsigned int error_check = jeffersonMethodBySeats(jefferson_assigned_seats,
                                                        total_seats,
                                                        population,
                                                        jefferson_seat_vector,
                                                        pop_vector,
                                                        debug);
      if (error_check > 0)
      {
        cout << "ERROR: Problem running Jefferson Method, closing program." << endl;
        return 0;
      }
      if (debug)
        cout << "Jefferson Method Finished\n"
             << "Running Webster Method\n";
      websterMethodBySeats(webster_assigned_seats,
                           total_seats,
                           webster_seat_vector,
                           pop_vector,
                           ratio);
      if (debug)
        cout << "Webster Method Finished\n";
    }
    else if (option == 2)
    {
      if (debug)
        cout << "Running Jefferson Method\n";
      jeffersonMethodByRatio(jefferson_assigned_seats,
                             jefferson_seat_vector,
                             pop_vector,
                             ideal_ratio);
      if (debug)
        cout << "Finished Jefferson Method\n"
             << "Running Webster Method\n";
      websterMethodByRatio(webster_assigned_seats,
                           webster_seat_vector,
                           pop_vector,
                           ratio);
      if (debug)
        cout << "Finished Webster Method\n";
    }
    if (debug)
      cout << "Running Hamilton Method\n";
    hamiltonMethod(hamilton_assigned_seats,
                   total_seats,
                   hamilton_seat_vector,
                   pop_vector,
                   ratio);
    if (debug)
      cout << "Finished Hamilton Method\n"
           << "Running Hunting-Hill Method\n";
    huntingtonHillMethod(huntington_hill_assigned_seats,
                         total_seats,
                         population,
                         ideal_ratio,
                         huntington_hill_seat_vector,
                         pop_vector);
    if (debug)
      cout << "Finished Huntington-Hill Method\n\n";
  }

  cout << "What is the name of the output file? (please end it with the .csv or .txt extension)\n\n";

  cin >> output_name;

  output_file.open(output_name);

  if (method == 9)
  {
    output_file << "State & Population & Jefferson & Hamilton & HuntingtonHill & Webster\n";
    for (unsigned int i = 0; i != pop_vector.size(); ++i)
	  {
		  output_file << name_vector[i] << " & "
			        		<< pop_vector[i] << " & "
					        << jefferson_seat_vector[i] << " & "
                  << hamilton_seat_vector[i] << " & "
                  << huntington_hill_seat_vector[i] << " & "
                  << webster_seat_vector[i] << '\n';
	  }
	  output_file << "Total & "
                << population << " & "
                << jefferson_assigned_seats << " & "
                << hamilton_assigned_seats << " & "
                << huntington_hill_assigned_seats << " & "
                << webster_assigned_seats << '\n';
	  output_file.close();
  }
  else
  {
    unsigned int red_ev = 0;
    unsigned int blue_ev = 0;
    unsigned int swing_ev = 0;
    output_file << "Name,Population,Seats,Electoral Votes\n";
    for (unsigned int i = 0; i != pop_vector.size(); ++i)
	  {
		  output_file << name_vector[i] << ","
			        		<< pop_vector[i] << ","
					        << seat_vector[i] << ","
                  << seat_vector[i] + 2 << '\n';
      if (abs(bias_vector[i]) < 4)
      {
        swing_ev += seat_vector[i] + 2;
      }
      else if (bias_vector[i] > 3)
      {
        blue_ev += seat_vector[i] + 2;
      }
      else
      {
        red_ev += seat_vector[i] + 2;
      }
	  }
    double red_percent = static_cast<double>(red_ev) / (assigned_seats + seat_vector.size() * 2);
    double blue_percent = static_cast<double>(blue_ev) / (assigned_seats + seat_vector.size() * 2);
    double swing_percent = static_cast<double>(swing_ev) / (assigned_seats + seat_vector.size() * 2);
	  output_file << "Total," << population << "," << assigned_seats << "," << assigned_seats + seat_vector.size() * 2 << '\n';
    output_file << "Red," << red_ev << "," << red_percent * 100 << "%" << '\n';
    output_file << "Blue," << blue_ev << "," << blue_percent * 100 << "%" << '\n';
    output_file << "Swing," << swing_ev << "," << swing_percent * 100 << "%" << '\n';
	  output_file.close();
  }

  return 0;
}
