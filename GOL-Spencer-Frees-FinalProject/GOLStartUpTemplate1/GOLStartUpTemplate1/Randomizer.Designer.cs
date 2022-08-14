
namespace GOLStartUpTemplate1
{
    partial class Randomizer
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
            this.okayButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.numericUpDownRandom = new System.Windows.Forms.NumericUpDown();
            this.RandomizerLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownRandom)).BeginInit();
            this.SuspendLayout();
            // 
            // okayButton
            // 
            this.okayButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okayButton.Location = new System.Drawing.Point(195, 226);
            this.okayButton.Name = "okayButton";
            this.okayButton.Size = new System.Drawing.Size(75, 23);
            this.okayButton.TabIndex = 0;
            this.okayButton.Text = "Okay\r\n";
            this.okayButton.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(276, 226);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 1;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // numericUpDownRandom
            // 
            this.numericUpDownRandom.DecimalPlaces = 2;
            this.numericUpDownRandom.Location = new System.Drawing.Point(195, 56);
            this.numericUpDownRandom.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericUpDownRandom.Name = "numericUpDownRandom";
            this.numericUpDownRandom.Size = new System.Drawing.Size(75, 20);
            this.numericUpDownRandom.TabIndex = 2;
            // 
            // RandomizerLabel
            // 
            this.RandomizerLabel.AutoSize = true;
            this.RandomizerLabel.Location = new System.Drawing.Point(74, 58);
            this.RandomizerLabel.Name = "RandomizerLabel";
            this.RandomizerLabel.Size = new System.Drawing.Size(115, 13);
            this.RandomizerLabel.TabIndex = 3;
            this.RandomizerLabel.Text = "Pick Randomizer Seed";
            // 
            // Randomizer
            // 
            this.AcceptButton = this.okayButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(352, 252);
            this.Controls.Add(this.RandomizerLabel);
            this.Controls.Add(this.numericUpDownRandom);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okayButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Randomizer";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Randomizer";
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownRandom)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button okayButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.NumericUpDown numericUpDownRandom;
        private System.Windows.Forms.Label RandomizerLabel;
    }
}