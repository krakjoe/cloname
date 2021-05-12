--TEST--
Closure Naming
--FILE--
<?php
class Foo {
    public function bar() {
        return function() {
            
        };
    }
}

function qux() {
    return function() {
        
    };
}

function() {
    
};
?>
--EXPECTF--
Closure@Foo::bar:%s001.php#4-6
Closure@qux:%s001.php#11-13
Closure@{main}:%s001.php#16-18
