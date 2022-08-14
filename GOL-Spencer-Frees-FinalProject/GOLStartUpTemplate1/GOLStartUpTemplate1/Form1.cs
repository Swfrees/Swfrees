using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOLStartUpTemplate1
{
    public partial class Form1 : Form
    {
        // The universe array
        bool[,] universe = new bool[Properties.Settings.Default.gridWidth, Properties.Settings.Default.gridHeight];
        bool[,] scratchPad = new bool[Properties.Settings.Default.gridWidth, Properties.Settings.Default.gridHeight];

        int newWidth = Properties.Settings.Default.gridWidth;
        int newHeight = Properties.Settings.Default.gridHeight;

        // Drawing colors
        Color gridColor = Properties.Settings.Default.gridColor;
        Color cellColor = Properties.Settings.Default.livingCellsColor;
        Color textColor = Color.Black;

        // The Timer class
        Timer timer = new Timer();

        // Generation count
        int generations = 0;

        bool isHudVisible = true;

        public Form1()
        {
            InitializeComponent();

            graphicsPanel1.BackColor = Properties.Settings.Default.panelBackgroundcolor;

            // Setup the timer
            timer.Interval = Properties.Settings.Default.timerInterval; // milliseconds
            timer.Tick += Timer_Tick;
            timer.Enabled = false; // start timer running
        }

        //Count Neighbors
        private int CountNeighborsFinite(int x, int y)
        {
            int count = 0;
            int xLen = universe.GetLength(0);
            int yLen = universe.GetLength(1);

            for (int yOffset = -1; yOffset <= 1; yOffset++)
            {
                for (int xOffset = -1; xOffset <= 1; xOffset++)
                {
                    int xCheck = x + xOffset;
                    int yCheck = y + yOffset;

                    // if xOffset and yOffset are both equal to 0 then continue
                    if (xOffset == 0 && yOffset == 0)
                    {
                        continue;
                    }

                    // if xCheck is less than 0 then continue
                    if (xCheck < 0)
                    {
                        continue;
                    }

                    // if yCheck is less than 0 then continue
                    if (yCheck < 0)
                    {
                        continue;
                    }

                    // if xCheck is greater than or equal too xLen then continue
                    if (xCheck >= xLen)
                    {
                        continue;
                    }

                    // if yCheck is greater than or equal too yLen then continue
                    if (yCheck >= yLen)
                    {
                        continue;
                    }

                    if (universe[xCheck, yCheck] == true) count++;
                }
            }
            return count;
        }

        // Calculate the next generation of cells
        private void NextGeneration()
        {
            for (int y = 0; y < universe.GetLength(1); y++)
            {
                // Iterate through the universe in the x, left to right
                for (int x = 0; x < universe.GetLength(0); x++)
                {
                    int count = CountNeighborsFinite(x, y);
                    //Apply Rules                    
                    //Turn cell on/off in scratchPad

                    // If cell in the universe is alive and has less than 2 neighbors, it should die in the next generation
                    if (universe[x,y] == true && count < 2)
                    {
                        scratchPad[x,y] = false;
                    }
                    // If cell in the universe is alive and has more than 3 neighbors, it should die in the next generation
                    else if (universe[x,y] == true  && count > 3)
                    {
                        scratchPad[x,y] = false;
                    }
                    // If cell in the universe is alive and has 2 or 3 neighbors, it should be alive in the next generation
                    else if (universe[x,y] == true && (count == 2 || count == 3))
                    {
                        scratchPad[x,y] = true;
                    }
                    // If cell in the universe is dead and has 3 neighbors, it should be alive in the next generation
                    else if (universe[x,y] == false && count == 3)
                    {
                        scratchPad[x,y] = true;
                    }
                    // If cell in the universe is dead and doesn't have 3 neighbors, it should be dead in the next generation
                    if (universe[x,y] == false && count != 3)
                    {
                        scratchPad[x,y] = false;
                    }

                }
            }

            //copy scratchPad into Universe
            bool[,] temp = universe;
            universe = scratchPad;
            scratchPad = temp;

            // Increment generation count
            generations++;

            // Update status strip generations and living cells
            livingCellsCounter();
        }

        // The event called by the timer every Interval milliseconds.
        private void Timer_Tick(object sender, EventArgs e)
        {
            NextGeneration();
            graphicsPanel1.Invalidate();
        }

        //----------------------Randomizer------------------
        //grab nested for loops
        private void timeRandomized()
        {
            Random Randomized = new Random();
            for (int y = 0; y < universe.GetLength(1); y++)
            {
                // Iterate through the universe in the x, left to right
                for (int x = 0; x < universe.GetLength(0); x++)
                {
                    if (Randomized.Next(0,2) == 0)
                    {
                        universe[x, y] = true;
                    }
                }
            }
        }
        //create instance of random class
        //Random() seeds from time
        //Random(int32) for input seed
        //create dialog box, weekly meeting 4
        //use inputted seed for overloaded constructor

        //call next on random class inside second for loop
        //Next(Int32, Int32) 0 for min 2 for max
        //if 0 then turn cell on

        // ---------------------count Living-----------------
        //double for loop
        //add it to the generations strip
        //call this method everywhere
        private void livingCellsCounter()
        {
            int livingCells = 0;
            for (int y = 0; y < universe.GetLength(1); y++)
            {
                // Iterate through the universe in the x, left to right
                for (int x = 0; x < universe.GetLength(0); x++)
                {
                    if (universe[x,y] == true)
                    {
                        livingCells++;
                        toolStripStatusLabelGenerations.Text = "Generations = " + generations.ToString() + " " + "Living Cells = " + livingCells.ToString();
                    }
                }
            }
            graphicsPanel1.Invalidate();
        }

        //dialog box in meeting 4 for the size change and interval change
        //update timer.interval with what the user typed in
        //newwidth and newhieght in the dialog box, then resize scratchPad and universe
        
        //Paints graphics panel
        private void graphicsPanel1_Paint(object sender, PaintEventArgs e)
        {
            //CHANGE THINGS TO FLOATS
            // Calculate the width and height of each cell in pixels
            // CELL WIDTH = WINDOW WIDTH / NUMBER OF CELLS IN X
            float cellWidth = (float)graphicsPanel1.ClientSize.Width / (float)universe.GetLength(0);
            // CELL HEIGHT = WINDOW HEIGHT / NUMBER OF CELLS IN Y
            float cellHeight = (float)graphicsPanel1.ClientSize.Height / (float)universe.GetLength(1);

            // A Pen for drawing the grid lines (color, width)
            Pen gridPen = new Pen(gridColor, 1);

            // A Brush for filling living cells interiors (color)
            Brush cellBrush = new SolidBrush(cellColor);

            // Iterate through the universe in the y, top to bottom
            for (int y = 0; y < universe.GetLength(1); y++)
            {
                // Iterate through the universe in the x, left to right
                for (int x = 0; x < universe.GetLength(0); x++)
                {
                    // A rectangle to represent each cell in pixels
                    //RECTF instead of Rectangle
                    RectangleF cellRect = RectangleF.Empty;
                    cellRect.X = x * cellWidth;
                    cellRect.Y = y * cellHeight;
                    cellRect.Width = cellWidth;
                    cellRect.Height = cellHeight;

                    // Fill the cell with a brush if alive
                    if (universe[x, y] == true)
                    {
                        e.Graphics.FillRectangle(cellBrush, cellRect);
                    }

                    // Outline the cell with a pen
                    e.Graphics.DrawRectangle(gridPen, cellRect.X, cellRect.Y, cellRect.Width, cellRect.Height);

                    Font font = new Font("Arial", 10f);

                    StringFormat stringFormat = new StringFormat();
                    stringFormat.Alignment = StringAlignment.Center;
                    stringFormat.LineAlignment = StringAlignment.Center;
                    int neighbors = CountNeighborsFinite(x,y);

                    if (isHudVisible == true)
                    {
                        if (universe[x, y] == true)
                        {
                            textColor = Color.LimeGreen;
                            Brush textBrush = new SolidBrush(textColor);
                            e.Graphics.DrawString(neighbors.ToString(), font, textBrush, cellRect, stringFormat);
                        }

                        if (universe[x, y] == false)
                        {
                            textColor = Color.DarkRed;
                            Brush textBrush = new SolidBrush(textColor);
                            e.Graphics.DrawString(neighbors.ToString(), font, textBrush, cellRect, stringFormat);
                        }
                    }
                }
            }

            //add bool for HUd at the top
            //make sure it isnt in the for loops
            //if isHUDBOOL is true then render HUD
            //menu items toogle the bool
            //if menu item is checked iis already a property that I should use

            // Cleaning up pens and brushes
            gridPen.Dispose();
            cellBrush.Dispose();
        }

        //Paints panel when clicked on
        private void graphicsPanel1_MouseClick(object sender, MouseEventArgs e)
        {
            //CHANGE MATH HERE TO USE FLOATS AS WELL
            // If the left mouse button was clicked
            if (e.Button == MouseButtons.Left)
            {
                // Calculate the width and height of each cell in pixels
                float cellWidth = (float)graphicsPanel1.ClientSize.Width / (float)universe.GetLength(0);
                float cellHeight = (float)graphicsPanel1.ClientSize.Height / (float)universe.GetLength(1);

                // Calculate the cell that was clicked in
                // CELL X = MOUSE X / CELL WIDTH
                float x = (float)e.X / cellWidth;
                // CELL Y = MOUSE Y / CELL HEIGHT
                float y = (float)e.Y / cellHeight;

                // Toggle the cell's state
                 universe[(int)x, (int)y] = !universe[(int)x, (int)y];

                livingCellsCounter();

                // Tell Windows you need to repaint
                graphicsPanel1.Invalidate();
            }
        }

        //Exit button code
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        //creates new graphics panel
        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int y = 0; y < universe.GetLength(1); y++)
            {
                // Iterate through the universe in the x, left to right
                for (int x = 0; x < universe.GetLength(0); x++)
                {
                    universe[x, y] = false;
                }
            }

            graphicsPanel1.Invalidate();
        }

        //Code for start button
        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            timer.Enabled = true;
            graphicsPanel1.Invalidate();
        }

        //Code for pausing the program
        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            timer.Enabled = false;
        }

        //Code for next button
        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            if (timer.Enabled == false)
            {
                NextGeneration();
                graphicsPanel1.Invalidate();
            }
        }

        //Code for randomizing with the seed
        private void seedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Randomizer Random = new Randomizer();
            if (DialogResult.OK == Random.ShowDialog())
            {
                Random RandomSeed = new Random(Random.randomizerNumber);
                for (int y = 0; y < universe.GetLength(1); y++)
                {
                    // Iterate through the universe in the x, left to right
                    for (int x = 0; x < universe.GetLength(0); x++)
                    {
                        if (RandomSeed.Next(0, 2) == 0)
                        {
                            universe[x, y] = true;
                        }
                    }
                }
                graphicsPanel1.Invalidate();
            }
        }

        //Opening and Saving
        //.CELL file type
        //Capital O not 0
        //currentRow += '.';

        //if row[0] = '!'
        //{
        //  continue
        //}

        //use same code from the top for the MAxWidth and MaxHeight

        //before while loop create int for the y position that we self incriment through the loop

        //Method for saving file
        private void saveFile()
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "All Files|*.*|Cells|*.cells";
            dlg.FilterIndex = 2; dlg.DefaultExt = "cells";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                StreamWriter writer = new StreamWriter(dlg.FileName);

                // Write any comments you want to include first.
                // Prefix all comment strings with an exclamation point.
                // Use WriteLine to write the strings to the file. 
                // It appends a CRLF for you.
                writer.WriteLine("!This is my comment.");

                // Iterate through the universe one row at a time.
                for (int y = 0; y < 20; y++)
                    //replace 20 with a variable for the height
                {
                    // Create a string to represent the current row.
                    String currentRow = string.Empty;

                    // Iterate through the current row one cell at a time.
                    for (int x = 0; x < 20; x++)
                        //replace 20 with a variable for the width
                    {
                        // If the universe[x,y] is alive then append 'O' (capital O)
                        // to the row string.
                        if (universe[x,y] == true)
                        {
                            currentRow += 'O';
                        }

                        // Else if the universe[x,y] is dead then append '.' (period)
                        // to the row string.
                        else if (universe[x,y] == false)
                        {
                            currentRow += '.';
                        }
                    }

                    // Once the current row has been read through and the 
                    // string constructed then write it to the file using WriteLine.
                    writer.WriteLine(currentRow);
                }
                // After all rows and columns have been written then close the file.
                writer.Close();
            }
        }

        //Method for opening file
        private void openFile()
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "All Files|*.*|Cells|*.cells";
            dlg.FilterIndex = 2;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                StreamReader reader = new StreamReader(dlg.FileName);

                // Create a couple variables to calculate the width and height
                // of the data in the file.
                int maxWidth = 0;
                int maxHeight = 0;
                int yPos = 0;
                // Iterate through the file once to get its size.
                while (!reader.EndOfStream)
                {
                    // Read one row at a time.
                    string row = reader.ReadLine();

                    // If the row begins with '!' then it is a comment
                    // and should be ignored.

                    if (row[0] == '!')
                    {
                        continue;
                    }

                    // If the row is not a comment then it is a row of cells.
                    // Increment the maxHeight variable for each row read.

                    if (row[0] != '!')
                    {
                        maxHeight++;

                        maxWidth = row.Length;
                    }
                    
                    // Get the length of the current row string
                    // and adjust the maxWidth variable if necessary.
                }

                // Resize the current universe and scratchPad
                // to the width and height of the file calculated above.
                bool[,] universe = new bool [maxWidth, maxHeight];
                bool[,] scratchPad = new bool[maxWidth, maxHeight];

                // Reset the file pointer back to the beginning of the file.
                reader.BaseStream.Seek(0, SeekOrigin.Begin);

                // Iterate through the file again, this time reading in the cells.
                while (!reader.EndOfStream)
                {
                    // Read one row at a time.
                    string row = reader.ReadLine();

                    // If the row begins with '!' then
                    // it is a comment and should be ignored.
                    if (row[0] == '!')
                    {
                        continue;
                    }

                    // If the row is not a comment then 
                    // it is a row of cells and needs to be iterated through.
                    for (int xPos = 0; xPos < row.Length; xPos++)
                    {
                        // If row[xPos] is a 'O' (capital O) then
                        // set the corresponding cell in the universe to alive.
                        if (row[xPos] == 'O')
                        {
                            universe[xPos, yPos] = true;
                        }

                        // If row[xPos] is a '.' (period) then
                        // set the corresponding cell in the universe to dead.
                        if (row[xPos] == '.')
                        {
                            universe[xPos, yPos] = false;
                        }
                        
                    }
                    yPos++;
                }

                // Close the file.
                reader.Close();
            }
        }

        //Code to randomize by time
        private void timeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            timeRandomized();
            graphicsPanel1.Invalidate();
        }

        //Code to open file
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFile();
        }

        //Code to save file
        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFile();
        }

        //Code to remove the grid
        private void gridToolStripMenuItem_Click(object sender, EventArgs e)
        {
            gridToolStripMenuItem.Checked = false;
            gridColor = Color.White;
            graphicsPanel1.Invalidate();
        }

        //Code to remove neighbor count
        private void neightborCountToolStripMenuItem_Click(object sender, EventArgs e)
        {
            isHudVisible = false;
            neightborCountToolStripMenuItem.Checked = false;
            graphicsPanel1.Invalidate();
        }

        //Code to change timer interval and the size of the grid
        private void optionsToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            Settings newSettings = new Settings();
            if (DialogResult.OK == newSettings.ShowDialog())
            {
                timer.Interval = newSettings.getMiliseconds;
                newWidth = newSettings.getWidth;
                newHeight = newSettings.getHeight;

                universe = new bool[newWidth, newHeight];
                scratchPad = new bool[newWidth, newHeight];
            }
            graphicsPanel1.Invalidate();
        }

        //Code to change grid color
        private void gridColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog gridColorDlg = new ColorDialog();
            gridColorDlg.Color = gridColor;
            if (DialogResult.OK == gridColorDlg.ShowDialog())
            {
                gridColor = gridColorDlg.Color;
            }
            graphicsPanel1.Invalidate();
        }

        //Code to change background color
        private void backgroundColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog backgroundColorDlg = new ColorDialog();
            backgroundColorDlg.Color = graphicsPanel1.BackColor;
            if (DialogResult.OK == backgroundColorDlg.ShowDialog())
            {
                graphicsPanel1.BackColor = backgroundColorDlg.Color;
            }
            graphicsPanel1.Invalidate();
        }

        //Code to change the living cells color
        private void livingCellsColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog livingCellDlg = new ColorDialog();
            livingCellDlg.Color = cellColor;
            if (DialogResult.OK == livingCellDlg.ShowDialog())
            {
                cellColor = livingCellDlg.Color;
            }
            graphicsPanel1.Invalidate();
        }

        //Code to have the settings save when program closes
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Properties.Settings.Default.gridWidth = newWidth;
            Properties.Settings.Default.gridHeight = newHeight;
            Properties.Settings.Default.timerInterval = timer.Interval;
            Properties.Settings.Default.panelBackgroundcolor = graphicsPanel1.BackColor;
            Properties.Settings.Default.gridColor = gridColor;
            Properties.Settings.Default.livingCellsColor = cellColor;

            Properties.Settings.Default.Save();
        }

        //Code for reloading settings
        private void reloadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.Reload();

            newWidth = Properties.Settings.Default.gridWidth;
            newHeight = Properties.Settings.Default.gridHeight;
            timer.Interval = Properties.Settings.Default.timerInterval;
            graphicsPanel1.BackColor = Properties.Settings.Default.panelBackgroundcolor;
            gridColor = Properties.Settings.Default.gridColor;
            cellColor = Properties.Settings.Default.livingCellsColor;
        }

        //Code for resetting the settings
        private void resetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.Reset();

            newWidth = Properties.Settings.Default.gridWidth;
            newHeight = Properties.Settings.Default.gridHeight;
            timer.Interval = Properties.Settings.Default.timerInterval;
            graphicsPanel1.BackColor = Properties.Settings.Default.panelBackgroundcolor;
            gridColor = Properties.Settings.Default.gridColor;
            cellColor = Properties.Settings.Default.livingCellsColor;
        }
    }
}
