import os
import re
import shutil
import tarfile
import subprocess
from collections import defaultdict

tar_gz_path = 'assignment_sources.tar.gz'
def clear_pycache(script_path):
    pycache_path = os.path.join(script_path, '__pycache__')
    if os.path.exists(pycache_path) and os.path.isdir(pycache_path):
        shutil.rmtree(pycache_path)
        print(f"Cleared pycache at {pycache_path}")


def extract_identifiers(file_content):
    identifiers = set()
    lines = file_content.split('\n')
    for line in lines:
        line = re.sub(r'".*?"|\'.*?\'|#.*|//.*|/\*.*?\*/', '', line)
        possible_identifiers = re.findall(r'\b[_a-zA-Z][_a-zA-Z0-9]*\b', line)
        identifiers.update(possible_identifiers)
    return sorted(identifiers)


def create_html_summary(directory, summary_name, file_name=None):
    all_identifiers = set()
    html_content = f"<html><head><title>Summary of {os.path.basename(directory)}</title></head><body><h1>Summary of {os.path.basename(directory)}</h1>"

    file_list = [file_name] if file_name else [f for f in os.listdir(directory) if
                                               os.path.isfile(os.path.join(directory, f))]

    for filename in file_list:
        file_path = os.path.join(directory, filename) if not file_name else filename
        if file_path.endswith(('.c', '.clj', '.ml', '.lp', '.py')):
            try:
                with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
                    content = file.read()
                line_count = content.count('\n') + 1
                identifiers = extract_identifiers(content)
                all_identifiers.update(identifiers)
                html_content += f"<h2><a href=\"{filename}\">{filename}</a> ({line_count} lines)</h2>\n<ul>"
                for identifier in identifiers:
                    html_content += f"<li>{identifier}</li>"
                html_content += "</ul>"
            except OSError as e:
                print(f"Error processing file {file_path}: {e}")

    html_content += "</body></html>"
    summary_file_path = os.path.join(directory, summary_name)
    with open(summary_file_path, 'w', encoding='utf-8') as file:
        file.write(html_content)
        print(f"Created summary at {summary_file_path}")


def create_index_html(parent_directory, assignments):
    index_content = "<html><head><title>Index of Summaries</title></head><body><h1>Index of Assignment Summaries</h1><ul>"
    for assignment, summary_name in assignments.items():
        summary_path = os.path.join(assignment, summary_name)
        index_content += f'<li><a href="{summary_path}">{summary_name}</a></li>'
    index_content += "</ul></body></html>"
    index_file_path = os.path.join(parent_directory, 'index.html')
    with open(index_file_path, 'w', encoding='utf-8') as file:
        file.write(index_content)
        print(f"Created index at {index_file_path}")


def create_tar_gz(parent_directory, assignments, output_filename='assignment_sources.tar.gz'):
    with tarfile.open(output_filename, "w:gz") as tar:
        index_path = os.path.join(parent_directory, 'index.html')
        tar.add(index_path, arcname='index.html')

        for assignment, summary_name in assignments.items():
            dir_path = os.path.join(parent_directory, assignment)
            summary_file_path = os.path.join(dir_path, summary_name)
            tar.add(summary_file_path, arcname=os.path.relpath(summary_file_path, parent_directory))

            for root, dirs, files in os.walk(dir_path):
                for file in files:
                    if file.endswith(('.c', '.clj', '.ml', '.lp', '.py')):
                        full_file_path = os.path.join(root, file)
                        tar.add(full_file_path, arcname=os.path.relpath(full_file_path, parent_directory))
                dirs[:] = []  # Do not descend into subdirectories

        print(f"Created archive at {output_filename}")


def process_directories(parent_directory):
    assignments = {
        'Lab1MarkAbbe': 'summary_a1.html',
        'Micro2MarkAbbe': 'summary_a2.html',
        'Lab3MarkAbbe': 'summary_a3.html',
        'Lab4MarkAbbe': 'summary_a4.html',
        'Micro5MarkAbbe': 'summary_a5.html'
    }
    for assignment, summary_name in assignments.items():
        path = os.path.join(parent_directory, assignment)
        create_html_summary(path, summary_name)
        print(f"Processed {assignment}")
    create_index_html(parent_directory, assignments)

def prompt_and_send_email(tar_gz_path):
    email_address = input("Please enter the recipient's email address: ")
    command = f'echo "Mark Abbe\'s tar.gz" | mutt -a "{tar_gz_path}" -s "Mark Abbe\'s tar.gz" -- {email_address}'
    subprocess.run(command, shell=True, check=True)

clear_pycache(os.getcwd())
script_location = os.getcwd()
parent_directory = os.path.abspath(os.path.join(script_location, '..'))
print(f"Script started. Parent directory: {parent_directory}")
process_directories(parent_directory)
create_tar_gz(parent_directory, {
    'Lab1MarkAbbe': 'summary_a1.html',
    'Micro2MarkAbbe': 'summary_a2.html',
    'Lab3MarkAbbe': 'summary_a3.html',
    'Lab4MarkAbbe': 'summary_a4.html',
    'Micro5MarkAbbe': 'summary_a5.html'
})

prompt_and_send_email(tar_gz_path)
