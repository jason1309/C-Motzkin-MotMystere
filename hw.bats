setup() {
    load 'bats/bats-support/load'
    load 'bats/bats-assert/load'
}

# Normal usage
# ------------

@test "./hw examples/4x4hw.txt" {
    run ./hw < examples/4x4hw.txt
    assert_success
    assert_output 'LINUX'
}

@test "./hw examples/5x5hw.txt" {
    run ./hw < examples/5x5hw.txt
    assert_success
    assert_output 'ZZZZ'
}

@test "./hw examples/fruits-legumes.txt" {
    run ./hw < examples/fruits-legumes.txt
    assert_success
    assert_output 'POMMEDETERRE'
}

@test "./hw examples/musique.txt" {
    run ./hw < examples/musique.txt
    assert_success
    assert_output 'MANDOLINE'
}
