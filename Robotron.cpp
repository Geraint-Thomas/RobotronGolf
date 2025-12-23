// Robotron.cpp :
// A basic C++ program that runs a console version of the game Robotron.

#include <iostream>

using namespace std;

int x = 0, y = 0; // Screen size position
int loc(int _x, int _y)
{
	return _x + (_y * x); // Calculate the position in the screen buffer
}

int main()
{
	int r = 0, j = 0, l = 0, h=0; //number of robots, junk piles and lives
	cout << "ROBOTRON\n";
	cout << "X:";
	cin >> x; // Input for X size
	cout << "Y:";
	cin >> y; // Input for Y size
	cout << "R:";
	cin >> r; // Input for number of robots
	cout << "J:";
	cin >> j; // Input for number of junk piles
	if ((x * y) < (r + j + 1)) // Check if the screen can fit all entities
	{
		cout << "SCREEN TOO SMALL!" << endl;
		return 1; // Exit if too many entities
	}
	cout << "L:";
	cin >> l; // Input for number of lives
	cout << "H:";
	cin >> h; // Input for hyperspace usage

	int px, py; // Player position
	px = x / 2; // Initialize player position in the middle of the screen
	py = y / 2;

	char* screen = new char[x * y]; // Dynamic allocation of screen buffer
	char* screen2 = new char[x * y]; // Second screen buffer for double buffering

	memset(screen, ' ', x * y); // Fill the screen with spaces

	screen[loc(px, py)] = '@'; // Place player on the screen

	//place robots on the screen
	for (int i = 0; i < r; i++)
	{
		int rx = rand() % x; // Random X position for robot
		int ry = rand() % y; // Random Y position for robot
		if (screen[loc(rx, ry)] == ' ')
		{
			screen[loc(rx, ry)] = '#'; // Place robot on the screen
		}
		else
		{
			i--; // If position is occupied, retry placing the robot
		}
	}

	//place junk piles on the screen
	for (int i = 0; i < j; i++)
	{
		int jx = rand() % x; // Random X position for junk
		int jy = rand() % y; // Random Y position for junk
		if (screen[loc(jx, jy)] == ' ')
		{
			screen[loc(jx, jy)] = '^'; // Place Junk on the screen
		}
		else
		{
			i--; // If position is occupied, retry placing the junk
		}
	}
	// Game loop
	while (l > 0 && r > 0) // Continue until lives or robots are exhausted
	{
		system("cls"); // Clear the console screen
		for (int i = 0; i < y; i++) // Print the screen buffer
		{
			for (int j = 0; j < x; j++)
			{
				cout << screen[loc(j, i)];
			}
			cout << endl;
		}
		cout << "Lives: " << l << " Robots: "<< r << " Hyperspace: "<<h<<endl;// Display remaining lives
		memset(screen2, ' ', x * y); // Fill the screen buffer with spaces
		char input;
		cin >> input; // Get user input for movement
		int new_px = px, new_py = py; // New player position based on input
		switch (input)
		{
		case 'w': new_py--; break; // Move up
		case 's': new_py++; break; // Move down
		case 'a': new_px--; break; // Move left
		case 'd': new_px++; break; // Move right
		case 'h': // Hyperspace
		{
			if (h == 0) continue; // If hyperspace is not available, ignore input
			while (screen[loc(new_px, new_py)] != ' ' && h>0) // Find a random empty space
			{
				new_px = rand() % x; // Random X position
				new_py = rand() % y; // Random Y position
			};
			h--; // Decrease hyperspace usage count
			break;
		}
		default: continue; // Ignore invalid input
		}

		if (new_px < 0 || new_px >= x || new_py < 0 || new_py >= y) continue; // Check bounds

		screen[loc(px, py)] = ' '; // Clear old player position

		if (screen[loc(new_px, new_py)] == '#') // If robot is encountered
		{
			l--; // Lose a life
			screen[loc(new_px, new_py)] = '@'; // Clear robot
			screen2[loc(new_px, new_py)] = '@'; // Update player position
			px = new_px;
			py = new_py;
		}
		else // If junk or space is encountered
		{
			screen[loc(new_px, new_py)] = '@'; // Clear junk pile
			screen2[loc(new_px, new_py)] = '@'; // Update player position
			px = new_px;
			py = new_py;
		}

		//move robots and copy over junk piles
		for (int i = 0; i < y; i++)
		{
			for (int j = 0; j < x; j++)
			{
				int lc = loc(j, i);

				if (screen[lc] == '#') // If a robot is present
				{
					int move_x = 0, move_y = 0; // Movement variables
					if (j > px) move_x = -1;
					else if (j < px) move_x = 1;
					if (i > py) move_y = -1;
					else if (i < py) move_y = 1;
					int new_rx = j + move_x;
					int new_ry = i + move_y;

					screen[loc(j, i)] = ' '; // Clear old robot position

					if (screen2[loc(new_rx, new_ry)] == '@') // If player is in the new position
					{
						l--; // Lose a life
					}
					else if (screen[loc(new_rx, new_ry)] == '^' || screen2[loc(new_rx, new_ry)] == '#') // If junk or another robot is in the new position
					{
						screen2[loc(new_rx, new_ry)] = '^'; // Robot becomes junk
					}
					else
					{
						screen2[loc(new_rx, new_ry)] = '#'; // Move robot to new position
					}
				}
				else if (screen[lc] == '^') // If a junk pile is present
				{
					screen2[lc] = screen[lc];
				}
			}
		}

		r = 0; // Reset robot count
		for (int i = 0; i < x * y; i++)
		{
			if (screen2[i] == '#') // Count remaining robots
			{
				r++;
			}
		}

		char* temp = screen; // Swap the screen buffers
		screen = screen2;
		screen2 = temp;

	}
	if (r == 0) // If all robots are destroyed
	{
		cout << "You win!" << endl;
	}
	else if (l == 0) // If player has no lives left
	{
		cout << "You lose!" << endl;
	}
	cout << "Game Over!" << endl;
	delete[] screen; // Free allocated memory for screen buffer
	delete[] screen2;

}
