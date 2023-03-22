int f()
{
    int x;
    x=5678;
    {
        x=1234;
        {

            x = 1234 - 1165;
            {
                int x;
                int y;
                y = 7 + x;
                x = y;
            }
        }
    }
    return x;
}

