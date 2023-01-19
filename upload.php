<?php
session_start();

$target_dir = $_POST['Directory']."/";
$target_file = $target_dir.basename($_FILES['uploadedFile']["name"]);

if(move_uploaded_file($_FILES['uploadedFile']['tmp_name'], $target_file))
{
	echo "success";
}
else
{
	echo "error";
}
header("Location: filesystem.html");
exit;
?>
