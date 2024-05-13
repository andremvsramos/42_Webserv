<?php
$filename = getenv('FILENAME'); // get the filename from the environment variable

$data_dir = '../Data/'; // the directory where the file is located

// Check if the filename is empty
if (empty($filename)) {
    echo "Error: Filename is not provided.";
    exit; // exit early to prevent further execution
}

$file_path = $data_dir . $filename;

// Check if it's a directory
if (is_dir($file_path)) {
    echo "Error: Filename corresponds to a directory, not a file.";
    exit;
}

if (file_exists($file_path)) {
    unlink($file_path);
    echo "File deleted successfully.";
} else {
    echo "File not found.";
}

?>
