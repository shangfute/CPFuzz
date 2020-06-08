# CPFuzz
CPFuzz: Combining Fuzzing and Falsification of Cyber-Physical Systems


## Installation Instructions

```bash
cd cpfuzz
make
sudo make install
```

## Running the Examples

```bash
python cpfuzz.py -f examples/spi/spi1.tst
```
Then, in another terminal:

```bash
./fuzz.sh
```

## Print the seeds and trace

```bash
./read_testcase out/queue
```
