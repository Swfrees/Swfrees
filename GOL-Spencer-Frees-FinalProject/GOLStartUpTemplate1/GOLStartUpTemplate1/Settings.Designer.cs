
namespace GOLStartUpTemplate1
{
    partial class Settings
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.millisecondsLabel = new System.Windows.Forms.Label();
            this.newWidthLabel = new System.Windows.Forms.Label();
            this.newHeightLabel = new System.Windows.Forms.Label();
            this.numericUpDownMilliseconds = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownWidth = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownHeight = new System.Windows.Forms.NumericUpDown();
            this.okayButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMilliseconds)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownHeight)).BeginInit();
            this.SuspendLayout();
            // 
            // millisecondsLabel
            // 
            this.millisecondsLabel.AutoSize = true;
            this.millisecondsLabel.Location = new System.Drawing.Point(12, 35);
            this.millisecondsLabel.Name = "millisecondsLabel";
            this.millisecondsLabel.Size = new System.Drawing.Size(194, 13);
            this.millisecondsLabel.TabIndex = 0;
            this.millisecondsLabel.Text = "Milliseconds Between New Generations";
            // 
            // newWidthLabel
            // 
            this.newWidthLabel.AutoSize = true;
            this.newWidthLabel.Location = new System.Drawing.Point(96, 88);
            this.newWidthLabel.Name = "newWidthLabel";
            this.newWidthLabel.Size = new System.Drawing.Size(107, 13);
            this.newWidthLabel.TabIndex = 1;
            this.newWidthLabel.Text = "Generate New Width";
            // 
            // newHeightLabel
            // 
            this.newHeightLabel.AutoSize = true;
            this.newHeightLabel.Location = new System.Drawing.Point(96, 136);
            this.newHeightLabel.Name = "newHeightLabel";
            this.newHeightLabel.Size = new System.Drawing.Size(110, 13);
            this.newHeightLabel.TabIndex = 2;
            this.newHeightLabel.Text = "Generate New Height";
            // 
            // numericUpDownMilliseconds
            // 
            this.numericUpDownMilliseconds.DecimalPlaces = 2;
            this.numericUpDownMilliseconds.Location = new System.Drawing.Point(212, 33);
            this.numericUpDownMilliseconds.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericUpDownMilliseconds.Name = "numericUpDownMilliseconds";
            this.numericUpDownMilliseconds.Size = new System.Drawing.Size(120, 20);
            this.numericUpDownMilliseconds.TabIndex = 3;
            this.numericUpDownMilliseconds.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // numericUpDownWidth
            // 
            this.numericUpDownWidth.Location = new System.Drawing.Point(209, 88);
            this.numericUpDownWidth.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericUpDownWidth.Name = "numericUpDownWidth";
            this.numericUpDownWidth.Size = new System.Drawing.Size(120, 20);
            this.numericUpDownWidth.TabIndex = 4;
            this.numericUpDownWidth.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // numericUpDownHeight
            // 
            this.numericUpDownHeight.Location = new System.Drawing.Point(212, 136);
            this.numericUpDownHeight.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericUpDownHeight.Name = "numericUpDownHeight";
            this.numericUpDownHeight.Size = new System.Drawing.Size(120, 20);
            this.numericUpDownHeight.TabIndex = 5;
            this.numericUpDownHeight.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // okayButton
            // 
            this.okayButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okayButton.Location = new System.Drawing.Point(321, 194);
            this.okayButton.Name = "okayButton";
            this.okayButton.Size = new System.Drawing.Size(75, 23);
            this.okayButton.TabIndex = 6;
            this.okayButton.Text = "Okay";
            this.okayButton.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(413, 194);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 7;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // Settings
            // 
            this.AcceptButton = this.okayButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(500, 220);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okayButton);
            this.Controls.Add(this.numericUpDownHeight);
            this.Controls.Add(this.numericUpDownWidth);
            this.Controls.Add(this.numericUpDownMilliseconds);
            this.Controls.Add(this.newHeightLabel);
            this.Controls.Add(this.newWidthLabel);
            this.Controls.Add(this.millisecondsLabel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Settings";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Settings";
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMilliseconds)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownHeight)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label millisecondsLabel;
        private System.Windows.Forms.Label newWidthLabel;
        private System.Windows.Forms.Label newHeightLabel;
        private System.Windows.Forms.NumericUpDown numericUpDownMilliseconds;
        private System.Windows.Forms.NumericUpDown numericUpDownWidth;
        private System.Windows.Forms.NumericUpDown numericUpDownHeight;
        private System.Windows.Forms.Button okayButton;
        private System.Windows.Forms.Button cancelButton;
    }
}