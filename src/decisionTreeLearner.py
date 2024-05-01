import math
from enum import Enum
from typing import List

class Action(Enum):
    DANCING = 0
    WANDERING = 1
    PATHFINDING = 2

class Attribute(Enum):
    FOOD_DIST = 1
    ENEMY_DIST = 0 
    SKILL_LEVEL = 2

class AttributeValue(Enum):
    LOW = 0
    MEDIUM = 1
    HIGH = 2

class Line:
    def __init__(self, action: Action, enemyDistance: int, foodDistance: int, skillLevel: int):
        self.attrValues = [0] * len(Attribute)
        self.action = action
        self.attrValues[Attribute.ENEMY_DIST.value] = enemyDistance
        self.attrValues[Attribute.FOOD_DIST.value] = foodDistance
        self.attrValues[Attribute.SKILL_LEVEL.value] = skillLevel

    def getValue(self, attribute: int):
        if(attribute == 0):
            if(self.attrValues[attribute]<55):
                return AttributeValue.LOW
            elif self.attrValues[attribute]<70:
                return AttributeValue.MEDIUM
            else:
                return AttributeValue.HIGH
        elif(attribute == 1):
            if(self.attrValues[attribute]<25):
                return AttributeValue.LOW
            elif self.attrValues[attribute]<100:
                return AttributeValue.MEDIUM
            else:
                return AttributeValue.HIGH
        else:
            if(self.attrValues[attribute]<=2):
                return AttributeValue.LOW
            elif(self.attrValues[attribute]<5):
                return AttributeValue.MEDIUM
            elif self.attrValues[attribute]>=5:
                return AttributeValue.HIGH

    def __str__(self):
        return str(self.action)

class DecisionNode:
    def __init__(self, name: str = None):
        self.name = name
        self.testValue = None
        self.action = None
        self.daughters = []

    def populateDaughters(self):
        for _ in range(3):
            self.daughters.append(DecisionNode())

    def __str__(self):
        s = self.name + " - "
        if self.action is not None:
            s += self.action
        else:
            s += str(self.testValue)
        return s

    def print(self, node, level):
        print("---------------- Learned Decision-Tree ---------------")
        self.printHelper(node, 0)
        print("------------------------------------------------------")

    def printHelper(self, node, level):
        name = node.name
        if node is not None and name is not None and name != "None":
            s = str(level) + " |"
            for _ in range(1, level + 1):
                s += "\t"
            s += str(node).strip()
            print(s)
            for n in node.daughters:
                self.printHelper(n, level + 1)

class DecisionTreeLearner:
    numValues = 8

    def __init__(self, file):
        self.fileName = file
        lines = self.readLog()
        attributesArray = [Attribute.ENEMY_DIST, Attribute.FOOD_DIST, Attribute.SKILL_LEVEL]
        self.root = self.makeTree(lines, attributesArray, DecisionNode("ROOT"))

    def makeTree(self, lines: List[Line], attributes: List[Attribute], decisionNode: DecisionNode, parentValue: str = None):
        initialEntropy = self.entropy(lines)

        if initialEntropy <= 0:
            if lines:
                decisionNode.action = lines[0].action.name
            return decisionNode

        lineCount = len(lines)
        bestInformationGain = 0.0
        bestSplitAttribute = None
        bestSets = []

        for attribute in attributes:
            sets = self.splitByAttribute(lines, attribute)
            overallEntropy = self.entropyOfSets(sets, lineCount)
            informationGain = initialEntropy - overallEntropy

            if informationGain > bestInformationGain:
                bestInformationGain = informationGain
                bestSplitAttribute = attribute
                bestSets = sets

        decisionNode.testValue = bestSplitAttribute

        decisionNode.populateDaughters()
        newAttributes = [a for a in attributes if a != bestSplitAttribute]

        if not newAttributes:
            possibleActions = [0] * len(Action)
            for line in lines:
                action = line.action
                possibleActions[action.value] += 1

            maxActionIndex = possibleActions.index(max(possibleActions))
            decisionNode.action = Action(maxActionIndex).name
            return decisionNode

        for i, set in enumerate(bestSets):
            if set:
                attributeValue = set[0].getValue(bestSplitAttribute.value)
                daughter = DecisionNode(AttributeValue(attributeValue).name)
                decisionNode.daughters[i] = daughter
                self.makeTree(set, newAttributes, daughter, AttributeValue(attributeValue).name)

        return decisionNode

    def splitByAttribute(self, lines: List[Line], attribute: Attribute):
        sets = [[] for _ in range(3)]
        for line in lines:
            set = sets[line.getValue(attribute.value).value]
            set.append(line)

        return sets

    def entropy(self, lines: List[Line]):
        lineCount = len(lines)

        if lineCount == 0:
            return 0

        actionTallies = [0] * len(Action)

        for line in lines:
            actionTallies[line.action.value] += 1

        actionCount = sum(1 for tally in actionTallies if tally > 0)

        if actionCount <= 1:
            return 0

        entropy = 0

        for actionTally in actionTallies:
            if actionTally > 0:
                proportion = actionTally / lineCount
                entropy -= proportion * self.log2(proportion)

        return entropy

    def entropyOfSets(self, sets, lineCount):
        entropy = 0.0

        for set in sets:
            proportion = len(set) / lineCount
            entropy -= proportion * self.entropy(set)

        return entropy

    def log2(self, x):
        if x == 0:
            return 0
        result = math.log10(x) / math.log(2.0)
        if math.isnan(result) or math.isinf(result) or (result == -0.0):
            return 0.0
        return result

    def readLog(self):
        fileStream = open(self.fileName, 'r')
        fileStream.readline()  # Delete header
        lines = []

        for line in fileStream:
            tokens = line.split(',')
            enemyDistance = tokens[0]
            foodDistance = tokens[1]
            skillLevel = tokens[2]
            actionToken = tokens[3].strip()
            currAction = Action[actionToken]
            ln = Line(currAction, int(float(enemyDistance) // 1), int(float(foodDistance) // 1), int(skillLevel))
            lines.append(ln)

        fileStream.close()
        return lines

# Usage:
learner = DecisionTreeLearner("paramaterizedMonsterVals.csv")
learner.root.print(learner.root, 0)
