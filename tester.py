import os
import shutil
import subprocess

PROGRAM_PATH = "/Users/nikhil/Documents/Code/CS360/MAPF "

# !!!!! TRUE IF YOU HAVE PANDAS AND OPENPYXL !!!!!
USE_PANDAS = True
CBS = "CBS"
PP = "Prioritized planning"
FILE_TO_CHECK = PP


def main(
        program_path=PROGRAM_PATH,
        use_pandas=USE_PANDAS,
        file_to_check=FILE_TO_CHECK
):
    # Clear and remake output dir each time
    if os.path.exists("test_out/"):
        shutil.rmtree("test_out/")
    os.mkdir("test_out/")

    result = []

    for file_index in range(1, 51):
        print("Resolving", file_index)

        # Test file path
        input_path = "test/test_" + str(file_index) + ".txt"
        # Test file output path
        output_path = "test_out/test_" + str(file_index) + "out.txt"
        # Pipe cout to file
        stdout_path = "test_out/test_" + str(file_index) + "std.txt"

        with open(stdout_path, "w+") as stdout_f:
            # https://stackoverflow.com/questions/880918/
            # how-to-redirect-the-output-of-exe-to-a-file-in-python
            args = program_path + " " + input_path + " " + output_path
            ret = subprocess.Popen(args, stdout=stdout_f, stderr=stdout_f).wait()
            # Non 0 exit code
            if ret != 0:
                print("Fatal error:", input_path)
            else:
                # Terribly inefficient,
                # should have piped to Python, then save file,
                # instead of save file, then re-read the entire file
                stdout_f.seek(0)
                # Read last line, ignore \n
                result.append(stdout_f.readlines()[-1][:-1])

    if use_pandas is False:
        for iloveicecream, pistachio in enumerate(result):
            print(iloveicecream+1, pistachio)
    else:
        # Automation to the extreme,
        # because I do not want to study fun algorithms,
        # therefore I am writing brain dead Python.
        import pandas as pd

        df = pd.read_excel("test/sum-of-costs.xlsx", skiprows=1, engine="openpyxl")

        # Code that nobody understands
        res_df = [int(each[-2:]) if each[:3] == "Sum"
                  else (-1 if each[:4] == "Fail"
                        else 9999)
                  for each in result]
        for iloveicecream, (pistachio, matcha) in enumerate(zip(res_df, df[file_to_check])):
            print(iloveicecream+1, "Yours:", pistachio, "Answer:", matcha, pistachio == matcha)


if __name__ == '__main__':
    print(os.getcwd())
    main()
