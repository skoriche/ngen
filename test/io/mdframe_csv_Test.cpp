#include "gtest/gtest.h"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#include "mdframe.hpp"

class mdframe_csv_Test : public ::testing::Test
{
  protected:
    mdframe_csv_Test()
        : tempfile(testing::TempDir())
    {}

    ~mdframe_csv_Test() override
    {}

    void SetUp() override
    {
        this->tempfile.append("mdframeTest_ioCSV.csv");
    }

    void TearDown() override
    {
        // fs::remove(this->tempfile);
    }

    fs::path tempfile;
};

/**
 * This unit test checks that `io::detail::cartesian_indices`
 * correctly generates lists of indices based on the
 * cartesian product of the dimensions given (i.e. the `shape`).
 */
TEST_F(mdframe_csv_Test, io_csv_cartesian)
{
    std::vector<size_t> shape { 1, 2, 3 };
    std::vector<size_t> index(3);
    std::vector<std::vector<size_t>> output;
    io::detail::cartesian_indices(shape, index, 0, output);

    EXPECT_EQ(output[0][0], 0);
    EXPECT_EQ(output[0][1], 0);
    EXPECT_EQ(output[0][2], 0);

    EXPECT_EQ(output[1][0], 0);
    EXPECT_EQ(output[1][1], 0);
    EXPECT_EQ(output[1][2], 1);

    EXPECT_EQ(output[2][0], 0);
    EXPECT_EQ(output[2][1], 0);
    EXPECT_EQ(output[2][2], 2);

    EXPECT_EQ(output[3][0], 0);
    EXPECT_EQ(output[3][1], 1);
    EXPECT_EQ(output[3][2], 0);

    EXPECT_EQ(output[4][0], 0);
    EXPECT_EQ(output[4][1], 1);
    EXPECT_EQ(output[4][2], 1);

    EXPECT_EQ(output[5][0], 0);
    EXPECT_EQ(output[5][1], 1);
    EXPECT_EQ(output[5][2], 2);
}

TEST_F(mdframe_csv_Test, io_csv)
{
    io::mdframe df;

    df.add_dimension("x", 2)
      .add_dimension("y", 2);

    df.add_variable<int>("x", { "x" })
      .add_variable<int>("y", { "y" })
      .add_variable<double>("v", {"x", "y"});

    for (size_t x = 0; x < 2; x++) {
        df["x"].insert({ x }, x);
        df["y"].insert({ x }, x);
        for (size_t y = 0; y < 2; y++) {
            df["v"].insert({ x, y }, x * y);
        }
    }

    df.to_csv(this->tempfile.string(), "x");
}