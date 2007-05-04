/*
Java Code to demonstrate the user of the Java Wrapper over the timer library
Copyright (C) 2005 Madhur Kumar Tanwani, Nikhil Deshpande

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

import java.io.*;
import highrestimer.*;

public class FileOpen
{
	private HighResTimer h;
	private double avg;
	int count;
	
	public FileOpen()
	{
		h = new HighResTimer();
		avg = 0.0;
		count = 0;
	}

	public double getAverage()
	{
		return avg / count;
	}
	
	public void run(String dir)
		throws IOException
	{
		File d = new File(dir);
		if (!d.isDirectory())
			return;
		dir = d.getAbsolutePath();
		String[] list = d.list();
		for (int i = 0; i < list.length; ++i)
		{
			String dirItem = dir + File.separatorChar + list[i];
			File f = new File(dirItem);
			if (f.isFile())
			{
				FileInputStream fs = null;
				h.start();
				try
				{
					fs = new FileInputStream(f);
					h.stop();
					count++;
					avg += h.getDifference();
					System.err.println(dirItem + "\t" + h.getDifference() + "\t" + "SUCCESS");
				}
				catch (Exception e)
				{
					h.stop();
					count++;
					avg += h.getDifference();
					System.out.println(dirItem + "\t" + h.getDifference() + "\t" + "FAILURE");
					System.err.println("Exception for " + dirItem + " Description: " + e.getMessage());
				}
				finally
				{
					if (fs != null)
						fs.close();
				}
			} else if (f.isDirectory())
			{
					try
					{
						run(dirItem);
					}
					catch (Exception e)
					{
						System.err.println("Exception for " + dirItem + " Description: " + e.getMessage());
					}
			}
		}
		
	}
	
	public void test_simple_file_open(String fileName) 
		throws IOException
	{
		File handleToFile = new File(fileName);
		if ( handleToFile.isDirectory() )
			return ; /*we expect only ONE FILE!!!*/
		
		for (int iI = 0; iI < 10; iI++) {
			FileInputStream fileContents = null;
			h.start();
			fileContents = new FileInputStream(handleToFile);
			h.stop();
			count++;			
			avg += h.getDifference();
			fileContents.close();
		}
		
	}

	public static final void main(String[] args)
	{
		try
		{
			FileOpen f = new FileOpen();
			if (args.length > 0)
			{
				f.test_simple_file_open(args[0]);
				System.out.println("Avg File Open Time = " 
					+ f.getAverage() + "seconds" ); 
					
				System.out.println("");
				
				f.run(args[1]);
				System.out.println("Average directory parse time = "
						+ f.getAverage() + "seconds");
			}
		}
		catch (Exception e)
		{
			System.out.println(e.getMessage());
		}
	}
}
