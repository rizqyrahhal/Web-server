<html>
    <body>
<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $name = $_POST['name'] ?? '';
    $email = $_POST['email'] ?? '';
    echo "Name: $name\n";
    echo "Email: $email\n";
} else {
    ?>
    <form method="POST">
        <label for="name">Name:</label>
        <input type="text" name="name" id="name" required><br>
        <label for="email">Email:</label>
        <input type="email" name="email" id="email" required><br>
        <input type="submit" value="Submit">
    </form>
    <?php
}
?>
</body>
</html>
    