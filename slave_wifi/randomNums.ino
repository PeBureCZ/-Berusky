void generateNewRandom()
{
 randomSeedNum = actualTime % 100; //this have to be in range from 0 to 99 due to "pseudoRandomNums" array!
}

int getRandomNum(int min, int max)
{
 randomSeedNum++;
 if (randomSeedNum > 99) randomSeedNum = 0;
 int numRange = max - min;
 return (max - min) * pseudoRandomNums[randomSeedNum] / 100; //eg. range from 0 to 20 => 20 * 35 / 99 = 7
}
